#include "ikgaitgenerator.ih"

void IKGaitGenerator::resetGait()
{
  cout << "Resetting gait" << endl;
  d_phase = 0;
  d_speed = 0;

  d_curStep.targetDir = d_curStep.targetFaceDir = Vector3d(0, 1, 0);
  d_curStep.height = d_curStep.length = d_curStep.width = 0;
  d_curStep.lEnd = d_curStep.rEnd = Vector3d(0, d_offsetY, d_offsetZ);

  d_curStep.turn = 0;
  d_curStep.hipAngle = 0;

  d_jointVelocities = VectorXd::Zero(Types::NJOINTS);

  d_lastTime = 0;

  d_stage = -1;
}
