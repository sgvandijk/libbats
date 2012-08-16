#include "trainer.ih"

void Trainer::resetJoints()
{
  Cerebellum& cer = SCerebellum::getInstance();
  //WorldModel& wm = SWorldModel::getInstance();
  AgentModel& am = SAgentModel::getInstance();
  
  for (unsigned j = 0; j < Types::NJOINTS; ++j)
  {
    double angle = am.getJoint((Types::Joint)j)->angle->getMu()(0);
    double speed = -0.1 * angle;
    cer.addAction(make_shared<MoveJointAction>((Types::Joint)j, speed));
  }
}

