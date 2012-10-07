#include "dribbleagent.ih"

void DribbleAgent::think()
{
  AgentModel& am = SAgentModel::getInstance();

  // Initialize gait generator parameters
  GaitParams gaitParameters;
  gaitParameters.params = VectorXd(6);
  gaitParameters.params <<
    0,1,0, // Walk direction
    0,1,0; // Face direction

  // Run gait generator
  d_gaitGenerator->run(&gaitParameters);

  // Get results
  VectorXd jointVelocities = d_gaitGenerator->getJointVelocities();

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

