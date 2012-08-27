#include "kalmanlocalizer.ih"

bool KalmanLocalizer::tryObserveGlobalRotation(Eigen::Matrix3d& transform)
{
  if (!d_haveNewVisionData)
    return false;
  
  //
  // Attempt to find a forward vector equivalent to (0,1,0) from the global
  // frame, but in the local frame.
  //
  Vector3d forward(0, 0, 0);
  unsigned forwardCount = 0;
  for (LandmarkPair const& p : d_forwardPairs)
  {
    if (p.first->isVisible && p.second->isVisible)
    {
      Vector3d pos1 = p.first->getPositionRaw();
      Vector3d pos2 = p.second->getPositionRaw();
      forward += (pos2 - pos1).normalized();
      ++forwardCount;
    }
  }
  
  //
  // If we don't have at least one forward vector, we cannot continue.
  //
  if (forwardCount == 0)
    return false;
  
  forward /= forwardCount;
  forward.normalize();
  
  Vector3d right(0, 0, 0);
  unsigned rightCount = 0;
  for (LandmarkPair const& pair : d_rightPairs)
  {
    if (pair.first->isVisible && pair.second->isVisible)
    {
      Vector3d pos1 = pair.first->getPositionRaw();
      Vector3d pos2 = pair.second->getPositionRaw();
      right += (pos2 - pos1).normalized();
      ++rightCount;
    }
  }
  
  //
  // If we didn't get any right vectors, try to get one from a cross product
  // of landmarks from one end of the field.
  //
  if (rightCount == 0)
  {
    // Check cross products
    for (LandmarkPairPair const& pairPair : d_rightCrossProductPairs)
    {
      if (pairPair.first.first->isVisible  && pairPair.first.second->isVisible &&
          pairPair.second.first->isVisible && pairPair.second.second->isVisible)
      {
        Vector3d a = pairPair.first.second->getPositionRaw()  - pairPair.first.first->getPositionRaw();
        Vector3d b = pairPair.second.second->getPositionRaw() - pairPair.second.first->getPositionRaw();
        right += a.cross(b).normalized();
        ++rightCount;
      }
    }
    if (rightCount == 0)
      return false;
  }

  right /= rightCount;
  right.normalize();

  Vector3d up = right.cross(forward);
    
  // The transform comprised of these three vectors represents the transform
  // from the world frame into the agent's torso frame
  Affine3d invTransform = Math::makeTransform(right, forward, up);
  
  // The inverse gives what we want: the transform from the agent's torso
  // frame into world coordinates (equivalent to the world frame's axis
  // position/orientation in the torso's frame)
  transform = invTransform.inverse().rotation();

  return true;
}
