#include "types.ih"
#include "../AgentModel/agentmodel.hh"

bool Types::isPlayer(Types::Object objectId)
{
  return isTeamMate(objectId) || isOpponent(objectId);
};

bool Types::isTeamMate(Types::Object objectId)
{
  return objectId >= Types::TEAMMATE1 && objectId <= Types::getMaxPlayer();
};

bool Types::isOpponent(Types::Object objectId)
{
  return objectId >= Types::OPPONENT1 && objectId <= Types::getMaxOpponent();
};

bool Types::isMe(Types::Object objectId)
{
  return objectId == bats::SAgentModel::getInstance().getObject();
};

bool Types::isKeeper(Types::Object objectId)
{
  return bats::Types::unumFor(objectId) == 1;
};

bool Types::isFlag(Types::Object objectId)
{
  switch (objectId)
  {
    case Types::FLAG1US:
    case Types::FLAG2US:
    case Types::FLAG1THEM:
    case Types::FLAG2THEM:
      return true;
    default:
      return false;
  }
};

Types::Object Types::getMaxPlayer()
{
  WorldModel& wm = SWorldModel::getInstance();
  return (Types::Object)(Types::TEAMMATE1 + wm.getNumberOfPlayers() - 1);
};

Types::Object Types::getMaxOpponent()
{
  return Types::OPPONENT11;
//   WorldModel& wm = SWorldModel::getInstance();
//   return (Types::Object)(Types::OPPONENT1 + wm.getNumberOfPlayers() - 1);
};

