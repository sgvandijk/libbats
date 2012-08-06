#include "beam.ih"

using namespace bats;

bool Beam::run()
{
  Localizer& loc = SLocalizer::getInstance();

  StateVar x = rf_cast<StateVarNode>(d_goal->findDeep("X"))->getVar();
  Range xrange = x.second;
  StateVar y = rf_cast<StateVarNode>(d_goal->findDeep("Y"))->getVar();
  Range yrange = y.second;
  StateVar z = rf_cast<StateVarNode>(d_goal->findDeep("Z"))->getVar();
  Range zrange = z.second;
  
  Vector3d pos = Vector3d(xrange.mean(), yrange.mean(), zrange.mean());

  loc.onBeam(new BeamEvent(pos));

  // This is faster than using the parser.
  d_action = new BeamAction(Vector3d(xrange.mean(), yrange.mean(), zrange.mean()));
  
  addToActionCommandBehaviors();
  
  return true;
}
