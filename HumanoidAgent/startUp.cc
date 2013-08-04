#include "humanoidagent.ih"

#define QUOTE_IMPL(mp_token) #mp_token
#define QUOTE(mp_token) QUOTE_IMPL(mp_token) 

void HumanoidAgent::startUp()
{
  SConf::initialize();
  Conf& conf = SConf::getInstance();
  
  if (d_confFile != "")
    conf.parseFile(d_confFile);
  else
    conf.parseFile(QUOTE(LIBBATS_DATADIR)"/xml/conf.xml");
  
  unsigned def = 0;
  d_minThinkTime = conf.getParam("/minthinktime", def);
  
  SAgentSocketComm::initialize();
  SCochlea::initialize();
  SWorldModel::initialize();

  SAgentSocketComm::getInstance().initSocket(d_host, d_port);
  SWorldModel::getInstance().setTeamName(d_teamName);
  SCochlea::getInstance().setTeamName(d_teamName);

  SAgentModel::initialize();
  SCerebellum::initialize();
  
  AgentModel& am = SAgentModel::getInstance();
  am.setUnum(d_unum);
  
  //  WorldModel& wm = SWorldModel::getInstance();
  SAgentSocketComm::getInstance().connect();

  shared_ptr<Predicate> scene = make_shared<Predicate>("scene");
  scene->pushLeaf("rsg/agent/" + am.getRSG());
    
  _debugLevel4(*scene);
  SAgentSocketComm::getInstance().send(scene);

  SClock::initialize<RC3DClock>();
  
  // Set blokcing mode.
  //  d_comm.setBlocking(true);
  
}
