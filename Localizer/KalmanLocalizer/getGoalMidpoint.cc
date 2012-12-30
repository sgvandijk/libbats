#include "kalmanlocalizer.ih"

Eigen::Vector3d KalmanLocalizer::getTheirGoalMidpointLocal() const
{
  return (d_goal1Them->getPositionLocal(/*zeroZ*/true)
        + d_goal2Them->getPositionLocal(/*zeroZ*/true)) / 2;
};

Eigen::Vector3d KalmanLocalizer::getOurGoalMidpointLocal() const
{
  return (d_goal1Us->getPositionLocal(/*zeroZ*/true)
        + d_goal2Us->getPositionLocal(/*zeroZ*/true)) / 2;
};

Eigen::Vector3d KalmanLocalizer::getTheirGoalMidpointGlobal() const
{
  return Vector3d(SWorldModel::getInstance().getFieldLength()/2, 0, 0);
};

Eigen::Vector3d KalmanLocalizer::getOurGoalMidpointGlobal() const
{
  return Vector3d(-SWorldModel::getInstance().getFieldLength()/2, 0, 0);
};
