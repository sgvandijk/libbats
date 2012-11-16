#include "ikgaitgenerator.ih"

void IKGaitGenerator::determineNextStep(VectorXd const& targetDir, VectorXd const& targetFaceDir)
{
  AgentModel& am = SAgentModel::getInstance();

  // Store previous step
  d_prevStep = d_curStep;

  // The targets of this step
  d_curStep.targetDir = targetDir;
  d_curStep.targetFaceDir = targetFaceDir;

  // Determine step properties
  d_curStep.length = d_speed * d_ellipseLength * d_curStep.targetDir.y();
  d_curStep.width = d_speed * d_sideWidth * d_curStep.targetDir.x();
  d_curStep.height = d_speed * d_ellipseHeight;

  // The feet start where they ended in the last step
  d_curStep.lStart = d_prevStep.lEnd;
  d_curStep.rStart = d_prevStep.rEnd;
  d_curStep.offsetZ = d_offsetZ;

  double turnSign = targetFaceDir.x() < 0 ? -1 : 1;
  d_curStep.turn = turnSign * min(
    d_maxTurnAngle,
    0.5 * fabs(atan2(d_curStep.targetFaceDir.x(), d_curStep.targetFaceDir.y()))
    );

  // TODO: put this calculation somewhere central (Util/AgentModel)
  d_curStep.hipAngle = fabs(d_curStep.turn);
}
