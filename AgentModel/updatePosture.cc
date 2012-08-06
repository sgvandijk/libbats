#include "agentmodel.ih"

void AgentModel::updatePosture(bool integrateControl, rf<BodyPart> torso, bool updateJoints)
{
  if (!torso)
    torso = d_torso;
    
  assert(torso);
  
  Cochlea& cochlea = bats::SCochlea::getInstance();
  
  list<rf<BodyPart> > partQueue;

  torso->transform.setIdentity();

  partQueue.push_back(torso);

  double dtControl = (SClock::getInstance().getTime() - d_tLastControl) / 0.02;
  while (!partQueue.empty())
  {
    rf<BodyPart> part = partQueue.front();
    partQueue.pop_front();

    // Limb: Determine transformation of all connected joints by
    // applying proper translation
    if (rf<Limb> limb = rf_dynamic_cast<Limb>(part))
    {
      // Loop over all joints
      for (vector<rf<Joint> >::iterator iter = limb->joints.begin();
        iter != limb->joints.end();
        ++iter)
      {
        rf<Joint> joint = *iter;
        // Transformation = that of limb plus translation to the joint
        joint->transform =
          limb->transform *
          Translation3d(joint->anchors.first);
        
        partQueue.push_back(joint);
      }
    }
    else
    {
      rf<Joint> joint = rf_dynamic_cast<Joint>(part);
      
      Vector4d info = cochlea.getInfo(cochlea.translateInfo(joint->name));
      double a = Math::degToRad(info[0]);
      double r = info[1];
      double t = info[2];
      if (updateJoints)
      {
        joint->angle->init(
          VectorXd::Constant(1,(integrateControl ? a + joint->control*dtControl : a)),
          VectorXd::Constant(1,1.0));
        joint->rate = r;
        joint->torque = t;
      }
      
      rf<BodyPart> part2 = joint->bodyPart;
      part2->transform = joint->transform
        * AngleAxisd(joint->angle->getMu()(0), joint->axis)
        * Translation3d(-joint->anchors.second);

      partQueue.push_back(part2);
    }
  }
}
