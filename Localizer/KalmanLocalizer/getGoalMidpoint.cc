#include "kalmanlocalizer.ih"

Eigen::Vector3d KalmanLocalizer::getTheirGoalMidpointLocal() const
{
  return (goal1Them->getPositionLocal(/*zeroZ*/true)
        + goal2Them->getPositionLocal(/*zeroZ*/true)) / 2;
};

Eigen::Vector3d KalmanLocalizer::getOurGoalMidpointLocal() const
{
  return (goal1Us->getPositionLocal(/*zeroZ*/true)
        + goal2Us->getPositionLocal(/*zeroZ*/true)) / 2;
};

Eigen::Vector3d KalmanLocalizer::getTheirGoalMidpointGlobal() const
{
  return Vector3d(SWorldModel::getInstance().getFieldLength()/2, 0, 0);
};

Eigen::Vector3d KalmanLocalizer::getOurGoalMidpointGlobal() const
{
  return Vector3d(-SWorldModel::getInstance().getFieldLength()/2, 0, 0);
};
