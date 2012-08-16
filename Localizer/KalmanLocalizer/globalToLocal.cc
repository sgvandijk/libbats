#include "kalmanlocalizer.ih"

void KalmanLocalizer::globalToLocal()
{
  AgentModel& am = bats::SAgentModel::getInstance();

  Transform3d invGlobalTransform = Transform3d(d_globalTransform.inverse());

  Vector3d localUp = Math::getUp(invGlobalTransform);
  Vector3d localRight = localUp.cross(Vector3d::UnitX().cross(localUp)).normalized();
  Vector3d localForward = localUp.cross(Vector3d::UnitY().cross(localUp)).normalized();
  
  Transform3d invLocalTransform = Math::makeTransform(localRight, localForward, localUp);
  d_localTransform = invLocalTransform.inverse();
  
  //cerr << "local  trans:" << endl << d_localTransform.matrix()  << endl;
  //cerr << "global trans:" << endl << d_globalTransform.matrix() << endl;
   
  Transform3d globalToLocal = d_localTransform * invGlobalTransform;
  
  Transform3d globalToLocalRot = Transform3d(globalToLocal.linear());
  Transform3d globalToLocalRotTrans = Transform3d(globalToLocalRot.matrix().transpose());
  
  Vector3d lookDir = d_localTransform * Math::getForward(am.getBodyPart(Types::HEAD)->transform);
  Vector3d lookDirPolar = Math::cartesianToPolar(lookDir);

  //
  // Determine local coordinates for all objects given the earlier computed
  // global coordinates.
  //
  for (shared_ptr<ObjectInfo> object : objects)
  {
    object->posVelLocal->init(joinPositionAndVelocityVectors( globalToLocal             * object->getPositionGlobal(),
                                                              globalToLocalRot          * object->getVelocityGlobal()),
                              joinPositionAndVelocityMatrices(globalToLocalRot.linear() * cutPositionMatrix(object->posVelGlobal->getSigma()) * globalToLocalRotTrans.linear(),
                                                              globalToLocalRot.linear() * cutVelocityMatrix(object->posVelGlobal->getSigma()) * globalToLocalRotTrans.linear()));

    if (object->isPlayer)
    {
      shared_ptr<PlayerInfo> player = static_pointer_cast<PlayerInfo>(object);
      
      if (player->lArmVisible)
        player->posLArmLocal = globalToLocalRot * player->posLArmGlobal;
      if (player->rArmVisible)
        player->posRArmLocal = globalToLocalRot * player->posRArmGlobal;
      if (player->lFootVisible)
        player->posLFootLocal = globalToLocalRot * player->posLFootGlobal;
      if (player->rFootVisible)
        player->posRFootLocal = globalToLocalRot * player->posRFootGlobal;

      // If the either foot isn't at least 35cm below the origin of the torso,
      // consider the player as having fallen over.
      player->fallen = (player->posLFootGlobal.z() > -0.35 || player->posRFootGlobal.z() > -0.35);
      
      Vector3d pos = player->getPositionLocal();
      Vector3d larm = pos + player->posLArmLocal;
      Vector3d rarm = pos + player->posRArmLocal;
      Vector3d lfoot = pos + player->posLFootLocal;
      Vector3d rfoot = pos + player->posRFootLocal;

      Vector3d forward1 =  Math::calcPerpend(lfoot - rarm);
      Vector3d forward2 = -Math::calcPerpend(rfoot - larm);
      if (forward1.norm() != 0 && forward2.norm() != 0)
      {
        Vector3d fd = (forward1.normalized() + forward2.normalized()) / 2;
        const double alpha = 0.1;
        player->faceDirLocal = (1.0 - alpha) * player->faceDirLocal + alpha * fd;
      }
    }
  
    // If we didn't see the object, but we estimate that it should be in field of view, it's dead
    if (!object->isVisible)
    {
      Vector3d objPos = object->posVelLocal->getMu().start<3>();
      Vector3d headToObj = objPos - am.getBodyPart(Types::HEAD)->transform.translation();
      Vector3d objDirPolar = Math::cartesianToPolar(headToObj);
      Vector3d diff = objDirPolar - lookDirPolar;
      // If within these ranges, we should have seen it
      if (fabs(diff[1]) < 50.0 / 180.0 * M_PI && fabs(diff[2]) < 50.0 / 180.0 * M_PI)
      {
        object->posVelGlobal->setSigma(MatrixXd::Identity(6,6) * 30.0);
        object->posVelLocal->setSigma(MatrixXd::Identity(6,6) * 30.0);
      }
    }

    // TODO is there a better way of type checking/casting here?
    if (object->isDynamic)
    {
      shared_ptr<DynamicObjectInfo> dynamicObject = static_pointer_cast<DynamicObjectInfo>(object);
      // 95% certainty radius is 3m, the object is dead (TODO: un-magic-numberfy)
      if (sqrt(dynamicObject->posVelGlobal->getSigma()(0, 0)) * 2 > 3.0)
        dynamicObject->isAlive = false;
    }
  }
}