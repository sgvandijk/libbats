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
  list<rf<BodyPart> > partQueue;
  partQueue.push_back(d_torso);
  
  while (!partQueue.empty())
  {
    rf<BodyPart> part = partQueue.front();
    partQueue.pop_front();
    while (rf<Joint> joint = rf_dynamic_cast<Joint>(part))
      part = joint->bodyPart;
    
    rf<Limb> limb = rf_dynamic_cast<Limb>(part);
    limb->relativeWeight = limb->weight / d_weight;
    for (vector<rf<Joint> >::iterator iter = limb->joints.begin(); iter != limb->joints.end(); ++iter)
    {
      rf<Joint> joint = *iter;
      partQueue.push_back(joint->bodyPart);
    }
  }
  
}
