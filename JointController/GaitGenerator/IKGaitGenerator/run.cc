#include "ikgaitgenerator.ih"

void IKGaitGenerator::run(JointControlParams *jcParams)
{
  // Cast parameters
  GaitParams *gaitParams = static_cast<GaitParams*>(jcParams);

  // Get modules
  Clock& clock = bats::SClock::getInstance();
  AgentModel& am = bats::SAgentModel::getInstance();

  // CHeck whether the last time this gaitgenerator ran was too long ago
  double now = clock.getTime();
  double delta = now - d_lastTime;

  cout << "Delta: " << delta << endl;

  if (now - d_lastTime > 0.05)
    resetGait();

  d_lastTime = now;

  // Extract parameters
  Vector3d targetPos = gaitParams->params.head<3>();
  Vector3d targetFaceDir = gaitParams->params.segment<3>(3);
  Vector3d targetDir = targetPos.normalized();

  cout << targetPos.transpose() << endl << targetFaceDir.transpose() << endl << targetDir << endl;

  // Update the walking speed to slowly go faster
  updateSpeed();

  // Update walking cycle phase
  int oldStage = d_stage;
  updatePhase();

  // Determine next step at end of step. End of step is signaled by
  // going from one stage (one foot is swing foot) to other stage
  // (other foor is swing foot)
  d_stage = (int)(d_phase / M_PI);
  if (d_stage != oldStage)
  {
    determineNextStep(targetDir, targetFaceDir);
    // Determine where the swing foot will end
    determineSwingFootEnd();
  }
  
  // Calculate desired joint angles for this time step
  VectorXd jointAngles = doStep();
    
  // Determine desired joint velocities. Velocities are per timestep,
  // so simply the difference between angles and targets
  VectorXd curJointAngles = am.getJointAngles();
  d_jointVelocities = (jointAngles - curJointAngles);
}
