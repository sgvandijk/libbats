#include "agentmodel.ih"

void AgentModel::updateCOM()
{
  list<shared_ptr<BodyPart> > partQueue;
  partQueue.push_back(d_torso);
  Vector3d com;
  com.setZero();
  
  while (!partQueue.empty())
  {
    shared_ptr<BodyPart> part = partQueue.front();
    partQueue.pop_front();
    while (shared_ptr<Joint> joint = dynamic_pointer_cast<Joint>(part))
      part = joint->bodyPart;
    
    shared_ptr<Limb> limb = dynamic_pointer_cast<Limb>(part);
    
    com += limb->transform.translation() * limb->relativeWeight;
    for (vector<shared_ptr<Joint> >::iterator iter = limb->joints.begin(); iter != limb->joints.end(); ++iter)
    {
      shared_ptr<Joint> joint = *iter;
      partQueue.push_back(joint->bodyPart);
    }
  }
  _debugLevel4(com);
  d_COM = com;
}
