#include "beam.ih"

using namespace bats;

bool Beam::run()
{
  Localizer& loc = SLocalizer::getInstance();

  StateVar x = static_pointer_cast<StateVarNode>(d_goal->findDeep("X"))->getVar();
  Range xrange = x.second;
  StateVar y = static_pointer_cast<StateVarNode>(d_goal->findDeep("Y"))->getVar();
  Range yrange = y.second;
  StateVar z = static_pointer_cast<StateVarNode>(d_goal->findDeep("Z"))->getVar();
  Range zrange = z.second;
  
  Vector3d pos = Vector3d(xrange.mean(), yrange.mean(), zrange.mean());

  loc.onBeam(make_shared<BeamEvent>(pos));

  // This is faster than using the parser.
  d_action = make_shared<BeamAction>(Vector3d(xrange.mean(), yrange.mean(), zrange.mean()));
  
  addToActionCommandBehaviors();
  
  return true;
}
