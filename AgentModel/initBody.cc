#include "agentmodel.ih"

void AgentModel::initBody()
{
  Conf& conf = SConf::getInstance();

  vector<XMLNode> players;

  string path = "/conf/agentmodel";
  XMLNodeSet set = conf.selectXPath(path);
  if (set.empty())
    throw runtime_error(string() + "AgentModel::initBody: '" + path + "' not found.");

  XMLNode am = set.front();
  d_rsg = am.getProp("rsg");
  
  path = "/conf/agentmodel/bodypart[@name='torso']";
  set = conf.selectXPath(path);
  if (set.empty())
    throw runtime_error(string() + "AgentModel::initBody: '" + path + "' not found.");
  
  XMLNode torso = set.front();
  d_weight = initBody(torso, 0);
  
  // Precalculate relative weight of parts
  list<shared_ptr<BodyPart> > partQueue;
  partQueue.push_back(d_torso);
  
  while (!partQueue.empty())
  {
    shared_ptr<BodyPart> part = partQueue.front();
    partQueue.pop_front();
    while (shared_ptr<Joint> joint = dynamic_pointer_cast<Joint>(part))
      part = joint->bodyPart;
    
    shared_ptr<Limb> limb = dynamic_pointer_cast<Limb>(part);
    limb->relativeWeight = limb->weight / d_weight;
    for (vector<shared_ptr<Joint> >::iterator iter = limb->joints.begin(); iter != limb->joints.end(); ++iter)
    {
      shared_ptr<Joint> joint = *iter;
      partQueue.push_back(joint->bodyPart);
    }
  }
  
}
