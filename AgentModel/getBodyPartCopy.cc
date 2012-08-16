#include "agentmodel.ih"

shared_ptr<BodyPart> AgentModel::getBodyPartCopy(shared_ptr<BodyPart> part)
{
  if (shared_ptr<Limb> l = dynamic_pointer_cast<Limb>(part))
  {
    shared_ptr<Limb> copy = make_shared<Limb>();
    copy->name = l->name;
    copy->weight = l->weight;
    copy->relativeWeight = l->relativeWeight;
    copy->size = l->size;
    for (unsigned j = 0; j < l->joints.size(); ++j)
      copy->joints.push_back(static_pointer_cast<Joint>(getBodyPartCopy(l->joints[j])));
    return copy;
  }
  else if (shared_ptr<Joint> j = dynamic_pointer_cast<Joint>(part))
  {
    shared_ptr<Joint> copy = make_shared<Joint>();
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
