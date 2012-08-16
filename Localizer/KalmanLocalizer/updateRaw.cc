#include "kalmanlocalizer.ih"

void KalmanLocalizer::updateRaw()
{
  // No sense in updating if there is no vision data
  if (!d_haveNewVisionData)
    return;
  
  Cochlea& cochlea = bats::SCochlea::getInstance();
  WorldModel& wm = bats::SWorldModel::getInstance();
  AgentModel& am = bats::SAgentModel::getInstance();
  
  Transform3d invHeadTransform = am.getBodyPart(Types::HEAD)->transform;
  
  //
  // Set everything as non-visible initially.  We will set these to true
  // as we process the raw data below.
  //
  for (shared_ptr<ObjectInfo> object : objects)
  {
    if (object->isPlayer)
    {
      shared_ptr<PlayerInfo> player = static_pointer_cast<PlayerInfo>(object);
      player->lArmVisible = false;
      player->rArmVisible = false;
      player->lFootVisible = false;
      player->rFootVisible = false;
    }
    object->isVisible = false;
  }
  
  //
  // Loop through all objects and see whether there's new vision data for them.
  //
  for (shared_ptr<ObjectInfo> object : objects)
  {
    // TODO: also filter noise
    
    //
    // Special handling for observed player limbs
    //
    if (object->isPlayer)
    {
      shared_ptr<PlayerInfo> player = static_pointer_cast<PlayerInfo>(object);
      
      //
      // We may be seeing our own limbs.  Ignore them.
      //
      if (player->isMe)
        continue;
      
      int pIdx = player->unum - 1;
      Cochlea::InfoID leftArmId   = (Cochlea::InfoID)((player->isOpponent ? Cochlea::iLLowerArmOpponent1 : Cochlea::iLLowerArmTeamMate1) + pIdx);
      Cochlea::InfoID rightArmId  = (Cochlea::InfoID)((player->isOpponent ? Cochlea::iRLowerArmOpponent1 : Cochlea::iRLowerArmTeamMate1) + pIdx);
      Cochlea::InfoID leftFootId  = (Cochlea::InfoID)((player->isOpponent ? Cochlea::iLFootOpponent1     : Cochlea::iLFootTeamMate1    ) + pIdx);
      Cochlea::InfoID rightFootId = (Cochlea::InfoID)((player->isOpponent ? Cochlea::iRFootOpponent1     : Cochlea::iRFootTeamMate1    ) + pIdx);

      if (wm.getTime() - cochlea.getTimestamp(leftArmId) < 0.005)
      {
        player->lArmVisible = true;
        Vector3d locPolar = cochlea.getInfo(leftArmId).start<3>();
        player->posLArmLocal = invHeadTransform * (Math::polarToCartesian(locPolar) + d_cameraOffset);
      }

      if (wm.getTime() - cochlea.getTimestamp(rightArmId) < 0.005)
      {
        player->rArmVisible = true;
        Vector3d locPolar = cochlea.getInfo(rightArmId).start<3>();
        player->posRArmLocal = invHeadTransform * (Math::polarToCartesian(locPolar) + d_cameraOffset);
      }

      if (wm.getTime() - cochlea.getTimestamp(leftFootId) < 0.005)
      {
        player->lFootVisible = true;
        Vector3d locPolar = cochlea.getInfo(leftFootId).start<3>();
        player->posLFootLocal = invHeadTransform * (Math::polarToCartesian(locPolar) + d_cameraOffset);
      }

      if (wm.getTime() - cochlea.getTimestamp(rightFootId) < 0.005)
      {
        player->rFootVisible = true;
        Vector3d locPolar = cochlea.getInfo(rightFootId).start<3>();
        player->posRFootLocal = invHeadTransform * (Math::polarToCartesian(locPolar) + d_cameraOffset);
      }
    }
    
    //
    // Update raw position of object. 
    //
    
    // Get the Cochlea's InfoID for this object
    Cochlea::InfoID infoId = getCochleaIdForObject(object);
    
    assert(infoId != Cochlea::iNone);
    
    // Do we have fresh vision data for this object?
    if (wm.getTime() - cochlea.getTimestamp(infoId) < 0.005)
    {
      object->isVisible = true;
      if (object->isDynamic)
        static_pointer_cast<DynamicObjectInfo>(object)->isAlive = true;

      Vector3d posPolar = cochlea.getInfo(infoId).start<3>();
      
      //
      // Use line sightings to reduce visual noise for flags, giving
      // up to three samples per flag instead of one.
      //
      if (object->isFlag)
      {
        int count = 0;
        Vector3d sum(0,0,0);        
        Vector3d posCartesian = Math::polarToCartesian(posPolar);
        for (Vector3d end : cochlea.getLineEnds())
        {
          Vector3d endCartesian = Math::polarToCartesian(end);
          const double flagToLineEndDistThreshold = 0.4;
          if ((endCartesian - posCartesian).norm() < flagToLineEndDistThreshold)
          {
            sum += end;
            count++;
          }
        }
        // Take the average
        if (count != 0)
          posPolar = (posPolar + sum) / (count + 1);
      }

      // sigma_dist     0.0965f * dist / 100.0
      // sigma_phi      0.1225f
      // sigma_theta    0.1480f
      const double distanceError = 0.0965; // must also be multiplied by distance/100
      const double horizontalError = 0.1225;
      const double verticalError = 0.1480;
      
      Vector3d noisePolar = Vector3d(
        distanceError * posPolar(0) / 100.0,
        Math::degToRad(horizontalError), 
        Math::degToRad(verticalError));
      
      VectorXd posVel(6);
      setPositionVector(posVel, invHeadTransform * Math::polarToCartesian(posPolar));

      VectorXd oldPosVel = object->posVelRaw->getMu();
      double dt = cochlea.getDt(infoId);
      setVelocityVector(posVel, cutPositionVector(posVel - oldPosVel) / dt);
      
      Vector3d posWithNoise =
        invHeadTransform *
        Math::polarToCartesian(posPolar + noisePolar);

      MatrixXd posVelSigma = MatrixXd::Zero(6,6);
      double s = (posWithNoise - cutPositionVector(posVel)).norm();
      posVelSigma.diagonal().setConstant(s);

      // Velocity uncertainty is sum of current measurement and
      // previous measurement This is mixing from previous and current
      // agent frames, though since we use the same value for each
      // dimension, it shouldn't matter much.
      posVelSigma.block<3,3>(3,3) =
        cutPositionMatrix(posVelSigma) / dt +
        cutPositionMatrix(object->posVelRaw->getSigma());

      object->posVelRaw->init(posVel, posVelSigma);
    }
  }
}
