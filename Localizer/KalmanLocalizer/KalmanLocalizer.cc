#include "kalmanlocalizer.ih"

KalmanLocalizer::KalmanLocalizer()
{
  WorldModel& wm = bats::SWorldModel::getInstance();
  double fieldLength = wm.getFieldLength();
  double fieldWidth = wm.getFieldWidth();
  double goalWidth = wm.getGoalWidth();
  double goalHeight = wm.getGoalHeight();
  
  double halfFieldLength = fieldLength / 2;
  double halfFieldWidth = fieldWidth / 2;
  double halfGoalWidth = goalWidth / 2;
  
  d_globalTransform = AngleAxisd(-.5 * M_PI, Vector3d::UnitZ());
  d_globalRotation = Transform3d(d_globalTransform.rotation());
  
  d_globalTranslation = Transform3d(MatrixXd::Identity(4,4));
  d_localTransform = Transform3d(MatrixXd::Identity(4,4));

  d_cameraOffset = Vector3d(0,0,0);
  
  const double playerRadius = 0.2;
  const double landmarkRadius = 0.045;
  
  //
  // Create the ball
  //
  objects.push_back(ball = new DynamicObjectInfo(bats::Types::BALL, wm.getBallRadius()));
  
  //
  // Create objects for players of both teams
  //
  // NOTE that we need to make all 11 opponents, even if we are playing with
  // fewer players, as teams may use any uniform number between 1 and 11,
  // though the keeper will always have unum==1.
  for (unsigned unum = 1; unum <= 11; ++unum)
    opponents.push_back(new PlayerInfo(Types::getOpponentWithUnum(unum), playerRadius));
  for (unsigned unum = 1; unum <= wm.getNumberOfPlayers(); ++unum)
    teamMates.push_back(new PlayerInfo(Types::getTeamMateWithUnum(unum), playerRadius));
  
  // Add them to the players list
  for (rf<PlayerInfo> teamMate : teamMates)
    players.push_back(teamMate);
  for (rf<PlayerInfo> opponent : opponents)
    players.push_back(opponent);
  for (rf<PlayerInfo> player : players)
  {
    if (player->isMe)
      me = player;
    objects.push_back(player);
  }
  
  landmarks.push_back(flag1Us   = d_objectInfos[Types::FLAG1US]     = new ObjectInfo(Types::FLAG1US,   /*radius*/landmarkRadius, /*dynamic*/false));
  landmarks.push_back(flag2Us   = d_objectInfos[Types::FLAG2US]     = new ObjectInfo(Types::FLAG2US,   /*radius*/landmarkRadius, /*dynamic*/false));
  landmarks.push_back(flag1Them = d_objectInfos[Types::FLAG1THEM]   = new ObjectInfo(Types::FLAG1THEM, /*radius*/landmarkRadius, /*dynamic*/false));
  landmarks.push_back(flag2Them = d_objectInfos[Types::FLAG2THEM]   = new ObjectInfo(Types::FLAG2THEM, /*radius*/landmarkRadius, /*dynamic*/false));
  landmarks.push_back(goal1Us   = d_objectInfos[Types::GOAL1US]     = new ObjectInfo(Types::GOAL1US,   /*radius*/landmarkRadius, /*dynamic*/false));
  landmarks.push_back(goal2Us   = d_objectInfos[Types::GOAL2US]     = new ObjectInfo(Types::GOAL2US,   /*radius*/landmarkRadius, /*dynamic*/false));
  landmarks.push_back(goal1Them = d_objectInfos[Types::GOAL1THEM]   = new ObjectInfo(Types::GOAL1THEM, /*radius*/landmarkRadius, /*dynamic*/false));
  landmarks.push_back(goal2Them = d_objectInfos[Types::GOAL2THEM]   = new ObjectInfo(Types::GOAL2THEM, /*radius*/landmarkRadius, /*dynamic*/false));
  landmarks.push_back(center    = d_objectInfos[Types::FIELDCENTER] = new ObjectInfo(Types::FIELDCENTER, /*radius*/landmarkRadius, /*dynamic*/false));
  
  // Populate the set of all landmarks
  for (rf<ObjectInfo> landmark : landmarks)
    objects.push_back(landmark);
  
  // Populate the set of all objects
  for (rf<ObjectInfo> object : objects)
    d_objectInfos[object->objectId] = object;
  
  //
  // Initialise fixed landmarks with known location details, and zero velocities, obviously 
  //
  flag1Us->posVelGlobal->init(  joinPositionAndVelocityVectors(Vector3d(-halfFieldLength,  halfFieldWidth, 0), Vector3d(0,0,0)), MatrixXd::Identity(6, 6));
  flag2Us->posVelGlobal->init(  joinPositionAndVelocityVectors(Vector3d(-halfFieldLength, -halfFieldWidth, 0), Vector3d(0,0,0)), MatrixXd::Identity(6, 6));
  flag1Them->posVelGlobal->init(joinPositionAndVelocityVectors(Vector3d( halfFieldLength,  halfFieldWidth, 0), Vector3d(0,0,0)), MatrixXd::Identity(6, 6));
  flag2Them->posVelGlobal->init(joinPositionAndVelocityVectors(Vector3d( halfFieldLength, -halfFieldWidth, 0), Vector3d(0,0,0)), MatrixXd::Identity(6, 6));
  
  goal1Us->posVelGlobal->init(  joinPositionAndVelocityVectors(Vector3d(-halfFieldLength,  halfGoalWidth, goalHeight), Vector3d(0,0,0)), MatrixXd::Identity(6, 6));
  goal2Us->posVelGlobal->init(  joinPositionAndVelocityVectors(Vector3d(-halfFieldLength, -halfGoalWidth, goalHeight), Vector3d(0,0,0)), MatrixXd::Identity(6, 6));
  goal1Them->posVelGlobal->init(joinPositionAndVelocityVectors(Vector3d( halfFieldLength,  halfGoalWidth, goalHeight), Vector3d(0,0,0)), MatrixXd::Identity(6, 6));
  goal2Them->posVelGlobal->init(joinPositionAndVelocityVectors(Vector3d( halfFieldLength, -halfGoalWidth, goalHeight), Vector3d(0,0,0)), MatrixXd::Identity(6, 6));

  center->posVelGlobal->init(   joinPositionAndVelocityVectors(Vector3d(0, 0, 0), Vector3d(0,0,0)), MatrixXd::Identity(6, 6));

  //
  // Store some known pairs of landmarks that can be used for orientation on
  // the field.
  //
  d_forwardPairs.push_back(LandmarkPair(flag2Us, flag1Us));
  d_forwardPairs.push_back(LandmarkPair(goal2Us, goal1Us));
  d_forwardPairs.push_back(LandmarkPair(flag2Them, flag1Them));
  d_forwardPairs.push_back(LandmarkPair(goal2Them, goal1Them));

  d_rightPairs.push_back(LandmarkPair(flag1Us, flag1Them));
  d_rightPairs.push_back(LandmarkPair(flag2Us, flag2Them));
  d_rightPairs.push_back(LandmarkPair(goal1Us, goal1Them));
  d_rightPairs.push_back(LandmarkPair(goal2Us, goal2Them));

  d_rightCrossProductPairs.push_back(LandmarkPairPair(LandmarkPair(flag1Us, goal1Us), LandmarkPair(flag1Us, goal2Us)));
  d_rightCrossProductPairs.push_back(LandmarkPairPair(LandmarkPair(flag1Us, goal1Us), LandmarkPair(flag1Us, flag2Us)));
  d_rightCrossProductPairs.push_back(LandmarkPairPair(LandmarkPair(flag1Us, goal2Us), LandmarkPair(flag1Us, flag2Us)));
  d_rightCrossProductPairs.push_back(LandmarkPairPair(LandmarkPair(goal1Us, goal2Us), LandmarkPair(goal1Us, flag2Us)));

  d_rightCrossProductPairs.push_back(LandmarkPairPair(LandmarkPair(flag1Them, goal1Them), LandmarkPair(flag1Them, goal2Them)));
  d_rightCrossProductPairs.push_back(LandmarkPairPair(LandmarkPair(flag1Them, goal1Them), LandmarkPair(flag1Them, flag2Them)));
  d_rightCrossProductPairs.push_back(LandmarkPairPair(LandmarkPair(flag1Them, goal2Them), LandmarkPair(flag1Them, flag2Them)));
  d_rightCrossProductPairs.push_back(LandmarkPairPair(LandmarkPair(goal1Them, goal2Them), LandmarkPair(goal1Them, flag2Them)));
}
