  #include "kalmanlocalizer.ih"

Cochlea::InfoID KalmanLocalizer::getCochleaIdForObject(rf<ObjectInfo> object) const
{
  if (object->isBall)
  {
    return Cochlea::iVisionBall;
  }
  
  if (object->isPlayer)
  {
    rf<PlayerInfo> player = rf_static_cast<PlayerInfo>(object);
    return (Cochlea::InfoID)((player->isTeamMate ? Cochlea::iVisionTeamMate1 : Cochlea::iVisionOpponent1) - 1 + player->unum);
  }
  
  // only landmarks left
  WorldModel& wm = bats::SWorldModel::getInstance();
  bool isLeft = wm.getSide() == Types::Side::LEFT;
  
  switch (object->objectId)
  {
    case Types::FLAG1US:
      return isLeft ? Cochlea::iVisionFlag1L : Cochlea::iVisionFlag2R;
      
    case Types::FLAG2US:
      return isLeft ? Cochlea::iVisionFlag2L : Cochlea::iVisionFlag1R;
      
    case Types::FLAG1THEM:
      return isLeft ? Cochlea::iVisionFlag1R : Cochlea::iVisionFlag2L;
      
    case Types::FLAG2THEM:
      return isLeft ? Cochlea::iVisionFlag2R : Cochlea::iVisionFlag1L;
      
    case Types::GOAL1US:
      return isLeft ? Cochlea::iVisionGoal1L : Cochlea::iVisionGoal2R;
      
    case Types::GOAL2US:
      return isLeft ? Cochlea::iVisionGoal2L : Cochlea::iVisionGoal1R;
      
    case Types::GOAL1THEM:
      return isLeft ? Cochlea::iVisionGoal1R : Cochlea::iVisionGoal2L;
      
    case Types::GOAL2THEM:
      return isLeft ? Cochlea::iVisionGoal2R : Cochlea::iVisionGoal1L;
      
    case Types::FIELDCENTER:
      return Cochlea::iVisionFieldCenter;
      
    default:
      assert(false);
  }
  
  return Cochlea::iNone;
}