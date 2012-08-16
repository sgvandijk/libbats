#include "kalmanlocalizer.ih"

void KalmanLocalizer::updatePlayersGlobal()
{
  VectorXd myPosVelGlobal = me->posVelGlobal->getMu();
  MatrixXd mySigma = me->posVelGlobal->getSigma();

  for (shared_ptr<PlayerInfo> player : players)
  {
    VectorXd oldPosVelGlobal = player->posVelGlobal->getMu();
    
    //
    // Predict
    //
    MatrixXd F = MatrixXd::Identity(6,6);
    
    double dt = 0.02;
    // x_{t+1} = x_t + dt * v_t
    // v_{t+1} = v_t
    //for (unsigned i = 0; i < 3; ++i)
    //  F(i, i+3) = dt;

    MatrixXd B = MatrixXd::Zero(6,3);
    
    // Assume player is decelerating
    // now: a = -v: assume decelerate to stop in a second
    // TODO: make this smarter
    Vector3d oldVel = cutVelocityVector(oldPosVelGlobal);
    Vector3d u = -.1 * oldVel;
    
    for (unsigned i = 0; i < 3; ++i)
    {
      B(i, i) = dt * dt / 2;
      B(i+3, i) = dt;
    }

    // Process noise to overcome lack of control data
    // TODO: tweak this
    // maximum estimated speed in meter per second
    double maxSpeed = 0.3;
    double maxDist = maxSpeed * dt;
    // If we set sigma to 1/2 * maxDist, 95% of distribution is within maximal distance
    // TODO: velocity change
    MatrixXd Q = MatrixXd::Identity(6, 6) * maxDist / 2;
    
    shared_ptr<NormalDistribution> controlModel = make_shared<NormalDistribution>(6);
    controlModel->init(B * u, Q);
    player->posVelGlobal->predict(F, controlModel);

    //
    // Update
    //
    if (d_haveNewVisionData)
    {
      /// TODO: cache obsModel
      shared_ptr<NormalDistribution> obsModel = make_shared<NormalDistribution>(6);
      /// TODO: cache trans of global rotation matrix
      Transform3d globalRotationTrans = Transform3d(d_globalRotation.matrix().transpose());
      if (player->isVisible)
      {
        VectorXd meas = player->posVelRaw->getMu();
        MatrixXd sigma = player->posVelRaw->getSigma();

        VectorXd globalMeas = joinPositionAndVelocityVectors(
          cutPositionVector(myPosVelGlobal) + d_globalRotation * cutPositionVector(meas),
          Vector3d(0,0,0));
        VectorXd vel = globalMeas - oldPosVelGlobal;
        globalMeas = joinPositionAndVelocityVectors(
          cutPositionVector(globalMeas), 
          cutPositionVector(vel));

        MatrixXd globalSigma = joinPositionAndVelocityMatrices(
          d_globalRotation.linear() * cutPositionMatrix(sigma) * globalRotationTrans.linear(),
          d_globalRotation.linear() * cutVelocityMatrix(sigma) * globalRotationTrans.linear()) + mySigma;

        obsModel->init(globalMeas, globalSigma);
        player->posVelGlobal->update(obsModel);
        
        player->isAlive = true;

        if (player->lArmVisible)
        {
          Vector3d rawDiff = player->posLArmLocal - meas.start<3>();
          Vector3d globalDiff = d_globalRotation * rawDiff;
          player->posLArmGlobal = globalDiff;
        }
        if (player->rArmVisible)
        {
          Vector3d rawDiff = player->posRArmLocal - meas.start<3>();
          Vector3d globalDiff = d_globalRotation * rawDiff;
          player->posRArmGlobal = globalDiff;
        }
        if (player->lFootVisible)
        {
          Vector3d rawDiff = player->posLFootLocal - meas.start<3>();
          Vector3d globalDiff = d_globalRotation * rawDiff;
          player->posLFootGlobal = globalDiff;
        }
        if (player->rFootVisible)
        {
          Vector3d rawDiff = player->posRFootLocal - meas.start<3>();
          Vector3d globalDiff = d_globalRotation * rawDiff;
          player->posRFootGlobal = globalDiff;
        }
      }
    }
  }
  
//  cout << "updatePlayersGlobal. Post: " << ball->getPositionLocal().transpose() << endl;
}
