#include "kalmanlocalizer.ih"

void KalmanLocalizer::updateGlobalRotation()
{
  // todo consider using an estimate of the gyro data for the *current* cycle -- find out whether the data tends to support this well or not
  
  Cochlea& cochlea = bats::SCochlea::getInstance();
  AgentModel& am = bats::SAgentModel::getInstance();
//  Debugger& dbg = bats::SDebugger::getInstance();

  //
  // Smooth the gyro data over two timesteps.  Units are radians per second.
  // The vector holds rotations around x, y and z axis.  They are to be applied
  // in that order (the order used by ODE in the simulator.)
  //
  Vector3d previousGyro = am.getPreviousGyro();
  Vector3d gyro = am.getGyro();
  Vector3d twoCycleAveragedGyroRotation = (previousGyro + gyro) / 2.0;
  
  //
  // Multiply radians per second by the time duration to get a value for the last cycle.
  //
  double dt = cochlea.getDt(Cochlea::iTorsoGyro);
  Vector3d rotationDelta = twoCycleAveragedGyroRotation * dt;

  //
  // Create a Affine3d representing the estimated rotational change in the timestep
  // prior to that in which the gyro data was measured.
  //
  Affine3d changeInRotation = Math::makeRotation(rotationDelta);

  //
  // Integrate this step change in rotation into our global rotation.
  //
  d_globalRotation = d_globalRotation * changeInRotation;

/*
  //
  // If the simulator is providing us with ground truth orientation, log out any difference.
  //
  // We may receive:
  //
  // 1. No ground truth data at all (it's received with vision data only every 3 cycles)
  // 2. Only the translation vector (the simulator isn't configured to report rotation)
  //    in which case the right/forward/up vectors are all zero
  // 3. The full transformation matrix, to two decimal places
  //
  Affine3d gtCameraToGlobalTransform = cochlea.getSelfTransform();
  
  Matrix3d gtCameraToGlobalRotation = gtCameraToGlobalTransform.matrix().block<3,3>(0,0);
  
  //
  // If the first 3x3 block is all zero, then we didn't receive any rotation data.
  //
  if (gtCameraToGlobalRotation.maxCoeff() != 0)
  {
    //
    // Ground truth reported by the visual system are in the camera's coordinate frame.
    // So, convert them to torso (the frame of the gyro).
    //
    Affine3d cameraToTorsoTransform = am.getBodyPart(Types::HEAD)->transform;
        
    Matrix4d gtAgentToGlobalMatrix = gtCameraToGlobalTransform * cameraToTorsoTransform.inverse();

    if (dbg.isEnabled() && dbg.isVerbose())
      dbg.draw(new TransformationAxes(gtAgentToGlobalMatrix, 0.4, "Rotation.Global.GroundTruth"));
    
    cout << "------------------------------------" << endl;
    
    //
    // Examine translation error
    //
    auto jointAngles = am.getJointAngles();
//     cout << "-- HJ1="<< jointAngles(0) << " ("<<am.getJoint(Types::HEAD1)->control<<") HJ2="<<jointAngles(1) << " ("<<am.getJoint(Types::HEAD2)->control<<")" << endl;
//     cout << "-- camera -> global (ground truth)" << endl << gtCameraToGlobalTransform.matrix() << endl;
//     cout << "-- camera -> agent (agent model) " << Math::radToDeg(AngleAxisd(cameraToTorsoTransform.matrix().block<3,3>(0,0)).angle()) << " deg on axis " << AngleAxisd(cameraToTorsoTransform.matrix().block<3,3>(0,0)).axis().transpose() << endl;
    cout << "-- agent -> global (ground truth) " << Math::radToDeg(AngleAxisd(gtAgentToGlobalMatrix.block<3,3>(0,0)).angle()) << " deg on axis " << AngleAxisd(gtAgentToGlobalMatrix.block<3,3>(0,0)).axis().transpose() << endl;
    cout << "-- agent -> global (belief) " << Math::radToDeg(AngleAxisd(d_globalRotation.matrix().block<3,3>(0,0)).angle()) << " deg on axis " << AngleAxisd(d_globalRotation.matrix().block<3,3>(0,0)).axis().transpose() << endl;
//     cout << "ROTATION " << Math::radToDeg(AngleAxisd(gtAgentToGlobalMatrix.block<3,3>(0,0)).angle()) << " " << Math::radToDeg(AngleAxisd(d_globalRotation.matrix().block<3,3>(0,0)).angle()) << endl;
//     cout << "-- belief - actual" << endl << (d_globalRotation.matrix() - gtAgentToGlobalMatrix).block<3,3>(0,0) << endl;
    
    // multiply one with the inverse of the other, giving the transformation between them
    Matrix3d rotationErrorMatrix = (d_globalRotation * gtAgentToGlobalMatrix.inverse()).block<3,3>(0,0);
//     cout << "-- rotation error matrix" << endl << rotationErrorMatrix << endl;

    AngleAxisd gtDiff = AngleAxisd(rotationErrorMatrix);
    cout << "-- agent -> global (belief difference to ground truth) " << Math::radToDeg(gtDiff.angle()) << " deg on axis " << gtDiff.axis().transpose() << endl << endl;

    //
    // Examine translation error
    //
    auto believedGlobalTorsoPosition = d_globalTransform.matrix().block<3,1>(0,3);
    auto actualGlobalCameraPosition = gtCameraToGlobalTransform.matrix().block<3,1>(0,3);
    auto torsoToCameraTranslation = cameraToTorsoTransform.matrix().block<3,1>(0,3);
    auto actualGlobalTorsoPosition = actualGlobalCameraPosition - torsoToCameraTranslation;
    auto globalTorsoPositionError = (believedGlobalTorsoPosition - actualGlobalTorsoPosition);
    cout << "-- agent's believed global torso position: " << believedGlobalTorsoPosition.transpose() << endl;
    cout << "-- agent's actual   global torso position: " << actualGlobalTorsoPosition.transpose() << endl;
    cout << "-- difference in    global torso position: " << globalTorsoPositionError.transpose() << " ("<< globalTorsoPositionError.norm() <<")" << endl;
  }
*/

//   if (dbg.isEnabled() && dbg.isVerbose())
//     dbg.draw(new TransformationAxes(d_globalRotation.matrix(), 0.4, "Rotation.Global.Estimate"));
  
  // if the global rotation differs greatly from what we see, use landmark
  // observations to set it correctly again.
  Eigen::Matrix3d observedGlobalRotation;
  if (tryObserveGlobalRotation(observedGlobalRotation))
  {
//    cout << "-- observed orientation" << endl << observedGlobalRotation << endl;
    
    // If both are the same, this should be the identity matrix
    Matrix3d obsDiffTransform = d_globalRotation.matrix().block<3,3>(0,0) * observedGlobalRotation.inverse();
    AngleAxisd obsDiff = AngleAxisd(obsDiffTransform);
//     cout << "-- observed difference: " << Math::radToDeg(obsDiff.angle()) << " deg on axis: " << obsDiff.axis().transpose() << endl << endl;

    AngleAxisd correction = AngleAxisd(-0.05 * obsDiff.angle(), obsDiff.axis());
    d_globalRotation = correction * d_globalRotation;
    
    //cout << "------" << endl;
    //cout << d_globalRotation.matrix() << endl << endl << transform.matrix() << endl << endl;
    //cout << obsDiff.matrix().diagonal().minCoeff() << endl;
    //cout << obsDiff.matrix() << endl;

    // NOTE commented out after fixing bug integration step, but needs revisiting
//   if (obsDiff.matrix().diagonal().minCoeff() < 0.96)
//   {
//     d_globalRotation = transform;//Affine3d(alpha * transform.matrix() + (1.0 - alpha) * d_globalRotation.matrix());
//   }
  }
}
