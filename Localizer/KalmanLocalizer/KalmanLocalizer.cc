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
  d_globalRotation = Affine3d(d_globalTransform.rotation());
  
  d_globalTranslation = Affine3d(Matrix4d::Identity());
  d_localTransform = Affine3d(Matrix4d::Identity());

  d_cameraOffset = Vector3d(0,0,0);
  
  const double playerRadius = 0.2;
  const double landmarkRadius = 0.045;
  
  //
  // Create the ball
  //
  d_objects.push_back(d_ball = make_shared<DynamicObjectInfo>(bats::Types::BALL, wm.getBallRadius()));
  
  //
  // Create objects for players of both teams
  //
  // NOTE that we need to make all 11 opponents, even if we are playing with
  // fewer players, as teams may use any uniform number between 1 and 11,
  // though the keeper will always have unum==1.
  for (unsigned unum = 1; unum <= wm.getNumberOfPlayers(); ++unum)
  {
    shared_ptr<PlayerInfo> tm(new PlayerInfo(Types::getTeamMateWithUnum(unum), playerRadius));
    if (tm->isMe)
      d_me = tm;
    d_teamMates.push_back(tm);
  }

  for (unsigned unum = 1; unum <= 11; ++unum)
    d_opponents.push_back(shared_ptr<PlayerInfo>(new PlayerInfo(Types::getOpponentWithUnum(unum), playerRadius)));
  
  // Add them to the players list
  d_players.insert(d_players.end(), d_teamMates.begin(), d_teamMates.end());
  d_players.insert(d_players.end(), d_opponents.begin(), d_opponents.end());

  d_objects.insert(d_objects.end(), d_players.begin(), d_players.end());
  
  // Populate the set of all landmarks
  d_landmarks.push_back(d_flag1Us =
			make_shared<ObjectInfo>(Types::FLAG1US,
						/*radius*/landmarkRadius,
						/*dynamic*/false));
  d_landmarks.push_back(d_flag2Us =
			make_shared<ObjectInfo>(Types::FLAG2US,
						/*radius*/landmarkRadius,
						/*dynamic*/false));
  d_landmarks.push_back(d_flag1Them =
			make_shared<ObjectInfo>(Types::FLAG1THEM,
						/*radius*/landmarkRadius,
						/*dynamic*/false));
  d_landmarks.push_back(d_flag2Them =
			make_shared<ObjectInfo>(Types::FLAG2THEM,
						/*radius*/landmarkRadius,
						/*dynamic*/false));
  d_landmarks.push_back(d_goal1Us   = 
			make_shared<ObjectInfo>(Types::GOAL1US,
						/*radius*/landmarkRadius,
						/*dynamic*/false));
  d_landmarks.push_back(d_goal2Us   =
			make_shared<ObjectInfo>(Types::GOAL2US,
						/*radius*/landmarkRadius,
						/*dynamic*/false));
  d_landmarks.push_back(d_goal1Them =
			make_shared<ObjectInfo>(Types::GOAL1THEM,
						/*radius*/landmarkRadius,
						/*dynamic*/false));
  d_landmarks.push_back(d_goal2Them =
			make_shared<ObjectInfo>(Types::GOAL2THEM,
						/*radius*/landmarkRadius,
						/*dynamic*/false));
  d_landmarks.push_back(d_center    =
			make_shared<ObjectInfo>(Types::FIELDCENTER,
						/*radius*/landmarkRadius,
						/*dynamic*/false));

  d_objects.insert(d_objects.end(), d_landmarks.begin(), d_landmarks.end());
  
  // Populate the set of all objects
  for (shared_ptr<ObjectInfo> object : d_objects)
    d_objectInfos[object->objectId] = object;
  
  //
  // Initialise fixed landmarks with known location details, and zero velocities, obviously 
  //
  d_flag1Us->posVelGlobal->init(joinPositionAndVelocityVectors(Vector3d(-halfFieldLength,  halfFieldWidth, 0), Vector3d(0,0,0)), MatrixXd::Identity(6, 6));
  d_flag2Us->posVelGlobal->init(  joinPositionAndVelocityVectors(Vector3d(-halfFieldLength, -halfFieldWidth, 0), Vector3d(0,0,0)), MatrixXd::Identity(6, 6));
  d_flag1Them->posVelGlobal->init(joinPositionAndVelocityVectors(Vector3d( halfFieldLength,  halfFieldWidth, 0), Vector3d(0,0,0)), MatrixXd::Identity(6, 6));
  d_flag2Them->posVelGlobal->init(joinPositionAndVelocityVectors(Vector3d( halfFieldLength, -halfFieldWidth, 0), Vector3d(0,0,0)), MatrixXd::Identity(6, 6));
  
  d_goal1Us->posVelGlobal->init(  joinPositionAndVelocityVectors(Vector3d(-halfFieldLength,  halfGoalWidth, goalHeight), Vector3d(0,0,0)), MatrixXd::Identity(6, 6));
  d_goal2Us->posVelGlobal->init(  joinPositionAndVelocityVectors(Vector3d(-halfFieldLength, -halfGoalWidth, goalHeight), Vector3d(0,0,0)), MatrixXd::Identity(6, 6));
  d_goal1Them->posVelGlobal->init(joinPositionAndVelocityVectors(Vector3d( halfFieldLength,  halfGoalWidth, goalHeight), Vector3d(0,0,0)), MatrixXd::Identity(6, 6));
  d_goal2Them->posVelGlobal->init(joinPositionAndVelocityVectors(Vector3d( halfFieldLength, -halfGoalWidth, goalHeight), Vector3d(0,0,0)), MatrixXd::Identity(6, 6));

  d_center->posVelGlobal->init(   joinPositionAndVelocityVectors(Vector3d(0, 0, 0), Vector3d(0,0,0)), MatrixXd::Identity(6, 6));

  //
  // Store some known pairs of landmarks that can be used for orientation on
  // the field.
  //
  d_forwardPairs.push_back(LandmarkPair(d_flag2Us, d_flag1Us));
  d_forwardPairs.push_back(LandmarkPair(d_goal2Us, d_goal1Us));
  d_forwardPairs.push_back(LandmarkPair(d_flag2Them, d_flag1Them));
  d_forwardPairs.push_back(LandmarkPair(d_goal2Them, d_goal1Them));

  d_rightPairs.push_back(LandmarkPair(d_flag1Us, d_flag1Them));
  d_rightPairs.push_back(LandmarkPair(d_flag2Us, d_flag2Them));
  d_rightPairs.push_back(LandmarkPair(d_goal1Us, d_goal1Them));
  d_rightPairs.push_back(LandmarkPair(d_goal2Us, d_goal2Them));

  d_rightCrossProductPairs.push_back(LandmarkPairPair(LandmarkPair(d_flag1Us, d_goal1Us), LandmarkPair(d_flag1Us, d_goal2Us)));
  d_rightCrossProductPairs.push_back(LandmarkPairPair(LandmarkPair(d_flag1Us, d_goal1Us), LandmarkPair(d_flag1Us, d_flag2Us)));
  d_rightCrossProductPairs.push_back(LandmarkPairPair(LandmarkPair(d_flag1Us, d_goal2Us), LandmarkPair(d_flag1Us, d_flag2Us)));
  d_rightCrossProductPairs.push_back(LandmarkPairPair(LandmarkPair(d_goal1Us, d_goal2Us), LandmarkPair(d_goal1Us, d_flag2Us)));

  d_rightCrossProductPairs.push_back(LandmarkPairPair(LandmarkPair(d_flag1Them, d_goal1Them), LandmarkPair(d_flag1Them, d_goal2Them)));
  d_rightCrossProductPairs.push_back(LandmarkPairPair(LandmarkPair(d_flag1Them, d_goal1Them), LandmarkPair(d_flag1Them, d_flag2Them)));
  d_rightCrossProductPairs.push_back(LandmarkPairPair(LandmarkPair(d_flag1Them, d_goal2Them), LandmarkPair(d_flag1Them, d_flag2Them)));
  d_rightCrossProductPairs.push_back(LandmarkPairPair(LandmarkPair(d_goal1Them, d_goal2Them), LandmarkPair(d_goal1Them, d_flag2Them)));
}
