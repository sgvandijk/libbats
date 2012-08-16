#include "beamto.ih"

BeamTo::BeamTo(string const &id, string const& behaviorTree)
  : Behavior("BeamTo", id, behaviorTree)
{
  d_tree = make_shared<AST::Node>(sequenceType);
  d_tree->addChild(make_shared<AST::Node>(andType));
  d_tree->getChild(0)->addChild(make_shared<AST::Node>(orType));
  d_tree->getChild(0)->addChild(make_shared<AST::Node>(orType));

  AgentModel& am = SAgentModel::getInstance();

  //Overwrite with specialized if possible
  std::ostringstream ids;
  ids << am.getUnum();
  
  d_posWeGetKickOff[0] = getParam("/where[@wegetkickoff=1 and @unum="+ids.str()+"]/@x", d_posWeGetKickOff[0]);
  d_posWeGetKickOff[1] = getParam("/where[@wegetkickoff=1 and @unum="+ids.str()+"]/@y", d_posWeGetKickOff[1]);
  d_posWeGetKickOff[2] = getParam("/where[@wegetkickoff=1 and @unum="+ids.str()+"]/@z", d_posWeGetKickOff[2]);
 
  d_posTheyGetKickOff[0] = getParam("/where[@wegetkickoff=0 and @unum="+ids.str()+"]/@x", d_posTheyGetKickOff[0]);
  d_posTheyGetKickOff[1] = getParam("/where[@wegetkickoff=0 and @unum="+ids.str()+"]/@y", d_posTheyGetKickOff[1]);
  d_posTheyGetKickOff[2] = getParam("/where[@wegetkickoff=0 and @unum="+ids.str()+"]/@z", d_posTheyGetKickOff[2]);
  
  d_coverOpponent = getParam("/where[@wegetkickoff=0 and @unum="+ids.str()+"]/@cover", 0) == 1;
  
}
