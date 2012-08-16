#include "movejointto.ih"

MoveJointTo::MoveJointTo(string const &id, string const& behaviorTree)
  : Behavior("MoveJointTo", id, behaviorTree),
    d_gain(0.1)
{
  d_gain = getParam("/gain", d_gain);

/*  XMLNodeSet params = getParams("/gain");
  
  if(params && !params.empty())
  {
    XMLNode gainNode = params.front();
    d_gain = atof(gainNode.getContent().c_str());
  }
*/

  XMLNodeSet params = getParams("/joint");
  
  if(!params || params.empty())
    throw new BatsException("MoveJointTo behavior requires a joint parameter");
  else
  {
    XMLNode jointNode = params.front();
    d_joint = (Types::Joint)atoi(jointNode.getContent().c_str());
  }
  
  d_tree = make_shared<AST::Node>(sequenceType);
  d_tree->addChild(make_shared<AST::Node>(andType));
  d_tree->getChild(0)->addChild(make_shared<AST::Node>(orType));
}

