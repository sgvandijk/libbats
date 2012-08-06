#include "robovizdebugger.ih"

void RoboVizDebugger::drawPlayerSkeleton(rf<bats::PlayerInfo> const info, float thickness, Eigen::Vector4d const& color, const std::string* setName)
{
  WorldModel& wm = bats::SWorldModel::getInstance();  

  Vector3d torso = wm.checkFlipSide(info->getPositionGlobal());
  Vector3d lArm = torso + info->posLArmGlobal;
  Vector3d rArm = torso + info->posRArmGlobal;
  Vector3d lFoot = torso + info->posLFootGlobal;
  Vector3d rFoot = torso + info->posRFootGlobal;

  if (info->lArmVisible)  drawLine(torso, lArm, thickness, color, setName);
  if (info->rArmVisible)  drawLine(torso, rArm, thickness, color, setName);
  if (info->lFootVisible) drawLine(torso, lFoot, thickness, color, setName);
  if (info->rFootVisible) drawLine(torso, rFoot, thickness, color, setName);

  if (info->lArmVisible  && info->rArmVisible)  drawLine(lArm, rArm, thickness, color, setName);
  if (info->rArmVisible  && info->lFootVisible) drawLine(rArm, lFoot, thickness, color, setName);
  if (info->lArmVisible  && info->lFootVisible) drawLine(lArm, lFoot, thickness, color, setName);
  if (info->rArmVisible  && info->rFootVisible) drawLine(rArm, rFoot, thickness, color, setName);
  if (info->lArmVisible  && info->rFootVisible) drawLine(lArm, rFoot, thickness, color, setName);
  if (info->lFootVisible && info->rFootVisible) drawLine(lFoot, rFoot, thickness, color, setName);
}
