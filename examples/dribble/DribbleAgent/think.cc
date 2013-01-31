#include "dribbleagent.ih"

void DribbleAgent::think()
{
  AgentModel& am = SAgentModel::getInstance();
  WorldModel& wm = SWorldModel::getInstance();
  Cerebellum& cer = SCerebellum::getInstance();
  
  VectorXd jointVelocities;

  if (!d_gettingUpFrom)
  {
    if (am.onMyBack())
      d_gettingUpFrom = 1;
    else if (am.onMyBelly())
      d_gettingUpFrom = 2;
  }

  if (d_gettingUpFrom)
  {
    /**********
     * GETTING UP
     **********/
    auto motionSequencePlayer = d_motionSequencePlayers[d_gettingUpFrom == 1 ? "getupfromback" : "getupfromfront"];
    motionSequencePlayer->run(0);
    jointVelocities = motionSequencePlayer->getJointVelocities();

    if (motionSequencePlayer->isSequenceDone())
    {
      motionSequencePlayer->reset();
      d_gettingUpFrom = 0;
    }
  }
  else
  {
    if (wm.getPlayMode() != Types::PLAY_ON)
    {
      /**********
       * STANDING
       **********/
      jointVelocities = stand();

      if (wm.getPlayMode() == Types::BEFORE_KICKOFF && !d_beamed)
      {
	cer.addAction(make_shared<BeamAction>(Vector3d(-3, 0, 0)));
	d_beamed = true;
      }
      
    }
    else
    {
      /**********
       * WALKING
       **********/
      VectorXd whereToWalkTo = determineWhereToWalk();
      
      // Initialize gait generator parameters
      GaitParams gaitParameters;
      gaitParameters.params = whereToWalkTo;
      
      // Run gait generator
      d_gaitGenerator->run(&gaitParameters);
      
      // Get results
      jointVelocities = d_gaitGenerator->getJointVelocities();
    }

    /**********
     * LOOKING
     **********/
    VectorXd currentJointAngles = am.getJointAngles();
    
    Vector2d whereToLookAt = determineWhereToLook();
    jointVelocities(Types::HEAD1) = whereToLookAt(0) - currentJointAngles(Types::HEAD1);
    jointVelocities(Types::HEAD2) = whereToLookAt(1) - currentJointAngles(Types::HEAD2);
  }

  // Add actions to the Cerebellum
  for (unsigned j = 0; j < Types::NJOINTS; ++j)
    cer.addAction(make_shared<MoveJointAction>((Types::Joint)j, jointVelocities(j)));
  
  // Tell Cerebellum to send the actions to the server
  cer.outputCommands(SAgentSocketComm::getInstance());
  
  //
  // Notify AgentModel of control
  //
  am.setControl(jointVelocities);
}

