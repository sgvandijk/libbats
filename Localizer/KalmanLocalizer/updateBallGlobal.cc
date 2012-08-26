#include "kalmanlocalizer.ih"

void KalmanLocalizer::updateBallGlobal()
{
  Cochlea& cochlea = bats::SCochlea::getInstance();
  WorldModel& wm = SWorldModel::getInstance();

  VectorXd oldLocVel = ball->posVelGlobal->getMu();
  
  /*
   * Predict
   */
  // F describes process without control: x_k+1 = x_k + dt v_k, v_k+1 = v_k
  MatrixXd F = MatrixXd::Identity(6, 6);
  
  double dt = 0.02;
  // x_{t+1} = x_t + dt * v_t
  // v_{t+1} = v_t
  for (unsigned i = 0; i < 3; ++i)
    F(i, i+3) = dt;
  //cerr << "F:" << endl << F << endl;
  
  // Control performed by drag
  // a = f/m = 0.01 * vel / 0.026
  // d x_k+1 = 1/2 a t^2
  // d v_k+1 = a t
  double linearDrag = -0.01;
  double ballMass = 0.026;
  Vector3d a = cutVelocityVector(oldLocVel) * linearDrag / ballMass;
  //cerr << "a:" << endl << a << endl;
  
  MatrixXd B = MatrixXd::Zero(6,3);
  VectorXd u = a;
  for (unsigned i = 0; i < 3; ++i)
  {
    B(i, i) = dt * dt / 2;
    B(i+3, i) = dt;
  }

  // Process noise
  // TODO: tweak this
  MatrixXd Q = MatrixXd::Identity(6, 6) * 0.0005;
  
  shared_ptr<NormalDistribution> controlModel = make_shared<NormalDistribution>(6);
  controlModel->init(B * u, Q);
  ball->posVelGlobal->predict(F, controlModel);

  /*
   * Update
   */
  if (d_haveNewVisionData)
  {
    // TODO: capture when we don't see ball
    if (ball->isVisible)
    {
      shared_ptr<NormalDistribution> obsModel = make_shared<NormalDistribution>(6);
      Affine3d globalRotationTrans = Affine3d(d_globalRotation.matrix().transpose());

      VectorXd meas = ball->posVelRaw->getMu();
      MatrixXd sigma = ball->posVelRaw->getSigma();

      VectorXd myPosVel = me->posVelGlobal->getMu();
      MatrixXd mySigma = me->posVelGlobal->getSigma();

      // Raw measurement of position in global coordinates
      VectorXd globalMeas = joinPositionAndVelocityVectors(
        cutPositionVector(myPosVel) + d_globalRotation * cutPositionVector(meas),
        Vector3d(0,0,0));
      VectorXd oldGlobalMeas = ball->posVelRawGlobal->getMu();
      
      // Determine velocity based on last global measurement
      VectorXd vel = VectorXd::Zero(3);
      // We know ball doesn't move in certain gamestates
      switch (wm.getPlayMode())
      {
      case Types::PLAY_ON:
	vel = (globalMeas - oldGlobalMeas) / cochlea.getDt(Cochlea::iVisionBall);
	break;

      default:
	break;
      }

      globalMeas = joinPositionAndVelocityVectors(
        cutPositionVector(globalMeas),
        cutPositionVector(vel));

      // Sigma of loc is transformed sigma of measurement
      // Sigma of vel is transformed sigma of measurement plus old sigma
      // Sigma_global = T^-1 Sigma_agent T, where T is transform from agent to global, i.e. inverse of global rotation
      Matrix3d posSigma =
	d_globalRotation.linear() * cutPositionMatrix(sigma) * globalRotationTrans.linear();
      // velocity uncertainty is uncertainty of 
      Matrix3d velSigma =
	d_globalRotation.linear() * cutVelocityMatrix(sigma) * globalRotationTrans.linear();

      MatrixXd globalSigma = joinPositionAndVelocityMatrices(posSigma, velSigma);

      // Remember for next time
      // Not pretty using posVelRawGlobal, as it is mixing raw and global.. 
      ball->posVelRawGlobal->init(globalMeas, globalSigma);

      obsModel->init(globalMeas, globalSigma);
      ball->posVelGlobal->update(obsModel);

    }
  }
}
