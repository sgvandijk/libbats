#include "robovizdebugger.ih"

const std::string RoboVizDebugger::getSetName(const std::string suffix)
{
  WorldModel& wm = SWorldModel::getInstance();
  AgentModel& am = SAgentModel::getInstance();

  unsigned unum = am.getUnum();
  string sideLabel = wm.getSide()==Types::LEFT ? "L" : "R";

  // todo prevent against illegal characters?
  ostringstream setName;
  setName << sideLabel << ".A" << unum;
  if (!suffix.empty())
    setName << "." << suffix;
  return setName.str();
}

const std::string RoboVizDebugger::getSetName(std::shared_ptr<Shape> shape)
{
  // If the shape specifies a category, use it as a suffix
  string suffix = shape->category;

  // Otherwise use "Extra"
  if (suffix.empty())
    suffix = "Extra";

  return getSetName(suffix);
}
