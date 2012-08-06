#include "agentmodel.ih"

rf<BodyPart> AgentModel::getBodyPartCopy(rf<BodyPart> part)
{
  if (rf<Limb> l = rf_dynamic_cast<Limb>(part))
  {
    rf<Limb> copy = new Limb();
    copy->name = l->name;
    copy->weight = l->weight;
    copy->relativeWeight = l->relativeWeight;
    copy->size = l->size;
    for (unsigned j = 0; j < l->joints.size(); ++j)
      copy->joints.push_back(rf_static_cast<Joint>(getBodyPartCopy(l->joints[j])));
    return copy;
  }
  else if (rf<Joint> j = rf_dynamic_cast<Joint>(part))
  {
    rf<Joint> copy = new Joint();
    copy->name = j->name;
    copy->axis = j->axis;
    copy->perceptor = j->perceptor;
    copy->effector = j->effector;
    copy->id = j->id;
    copy->type = j->type;
    copy->ujindex = j->ujindex;
    copy->angle->init(j->angle->getMu(), j->angle->getSigma());
    copy->bodyPart = getBodyPartCopy(j->bodyPart);
    copy->anchors = j->anchors;
    return copy;
  }
  return 0;
}
