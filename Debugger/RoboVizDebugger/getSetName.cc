#include "robovizdebugger.hh"
#include "robovizdebugger.ih"

const std::string RoboVizDebugger::getSetName(const std::string suffix)
{
  bats::WorldModel& wm = SWorldModel::getInstance();
  bats::AgentModel& am = SAgentModel::getInstance();

  unsigned unum = am.getUnum();
  std::string sideLabel = wm.getSide()==Types::LEFT ? "L" : "R";

  // TODO prevent against illegal characters?
  ostringstream setName;
  setName << sideLabel << ".A" << unum;
  if (!suffix.empty())
    setName << "." << suffix;
  return setName.str();
}

const std::string RoboVizDebugger::getSetName(bats::rf<Shape> shape)
{
  // If the shape specifies a category, use it as a suffix
  std::string suffix = shape->category;

  // Otherwise use "Extra"
  if (suffix.empty())
    suffix = "Extra";

  return getSetName(suffix);
}
