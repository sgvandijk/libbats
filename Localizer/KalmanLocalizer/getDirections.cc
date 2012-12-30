#include "kalmanlocalizer.ih"

Eigen::Vector3d KalmanLocalizer::getForwardDirLocal() const
{
  // TODO can't we do this from the global rotation matrix?
  Eigen::Vector3d theirGoalMidpointLocal = getTheirGoalMidpointLocal();
  Eigen::Vector3d ourGoalMidpointLocal = getOurGoalMidpointLocal();
  Eigen::Vector3d line = theirGoalMidpointLocal - ourGoalMidpointLocal;
//   if (fabs(line.norm() - bats::SWorldModel::getInstance().getFieldLength()) > 1)
//     std::cerr << "Line between goal midpoints has unexpected length: " << line.norm() << std::endl;
  return line.normalized();
};

Eigen::Vector3d KalmanLocalizer::getRightDirLocal() const
{
  // TODO can't we do this from the global rotation matrix?
  
  Eigen::Vector3d line = d_goal1Us->getPositionLocal(/*zeroZ*/true) 
                       - d_goal2Us->getPositionLocal(/*zeroZ*/true);

//   if (fabs(line.norm() - bats::SWorldModel::getInstance().getGoalWidth()) > 1)
//     std::cerr << "Line between goal posts has unexpected length: " << line.norm() << std::endl;

  return line.normalized();
};
