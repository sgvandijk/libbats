#include "playerinfo.hh"

#include "../../../WorldModel/worldmodel.hh"

bats::PlayerInfo::PlayerInfo(Types::Object objectId, double radius)
: DynamicObjectInfo(objectId, radius),
  posLArmLocal(Eigen::Vector3d(0,0,0)),
  posRArmLocal(Eigen::Vector3d(0,0,0)),
  posLFootLocal(Eigen::Vector3d(0,0,0)),
  posRFootLocal(Eigen::Vector3d(0,0,0)),
  posLArmGlobal(Eigen::Vector3d(0,0,0)),
  posRArmGlobal(Eigen::Vector3d(0,0,0)),
  posLFootGlobal(Eigen::Vector3d(0,0,0)),
  posRFootGlobal(Eigen::Vector3d(0,0,0)),
  isTeamMate(Types::isTeamMate(objectId)),
  isOpponent(Types::isOpponent(objectId)),
  isMe(Types::isMe(objectId)),
  isKeeper(Types::isKeeper(objectId)),
  unum(Types::unumFor(objectId))
{
//   std::cout << Types::nameOf(objectId) << " (id=" << objectId << ") unum=" << unum << " isOpponent=" << isOpponent << " isTeamMate=" << isTeamMate << std::endl;
  
  assert(isTeamMate ^ isOpponent);
  assert(unum > 0);
  assert(isOpponent || unum <= SWorldModel::getInstance().getNumberOfPlayers());
}