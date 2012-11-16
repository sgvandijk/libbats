#include "dribbleagent.ih"

void DribbleAgent::think()
{
  AgentModel& am = SAgentModel::getInstance();
  WorldModel& wm = SWorldModel::getInstance();
  
  VectorXd jointVelocities;
  if (wm.getPlayMode() != Types::PLAY_ON)
  {
    /**********
     * STANDING
     **********/
    jointVelocities = stand();
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

  // Add actions to the Cerebellum
  Cerebellum& cer = SCerebellum::getInstance();
  for (unsigned j = 0; j < Types::NJOINTS; ++j)
    cer.addAction(make_shared<MoveJointAction>((Types::Joint)j, jointVelocities(j)));
  
  // Tell Cerebellum to send the actions to the server
  cer.outputCommands(SAgentSocketComm::getInstance());

  //
  // Notify AgentModel of control
  //
  am.setControl(jointVelocities);

}

