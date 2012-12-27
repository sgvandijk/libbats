#include "dribbleagent.ih"

VectorXd DribbleAgent::determineWhereToWalk()
{
  VectorXd params(6);

  Localizer& localizer = SLocalizer::getInstance();
  
  // Determine where the ball is
  Vector3d ballPosition = localizer.ball->getPositionLocal(true);

  // Determine where the opponent's goal is
  Vector3d goalPosition = localizer.getTheirGoalMidpointLocal();

  Vector3d selfPosition(0,0,0);

  // Check whether we are on the right side:
  // ball between us and the goal
  bool onRightSide = Math::atSameSideOf(selfPosition, ballPosition, goalPosition);

  // Check whether we are in line with the goal. Project line from
  // player through ball onto the back line, and see if point is not
  // too far from the middel of the goal. Check whether the ball isn't
  // too far to the side.
  Vector3d backLineDirection = localizer.goal2Them->getPositionLocal() - localizer.goal1Them->getPositionLocal();
  Vector3d projection =
    Math::intersectLines2D(selfPosition, ballPosition,
			   goalPosition, backLineDirection);

  bool inLine = (projection - goalPosition).norm() < 1.0;

  bool centered = fabs(ballPosition.x()) < 0.1;

  if (onRightSide && /*inLine &&*/ centered)
  {
    // Aim somewhere behind the ball
    params.head<3>() = ballPosition.normalized() * 2;
    
    // Face the target
    params.tail<3>() = goalPosition;
  }
  else
  {
    // Go to a position behind the ball
    Vector3d ballToGoalDirection = (goalPosition - ballPosition).normalized();
    Vector3d behindBallPosition = ballPosition - 0.25 * ballToGoalDirection;
    params.head<3>() = behindBallPosition;
    
    // Face somewhere between the position behind the ball, and the
    // target, depending on how close we are to the ball.
    double faceFactor = behindBallPosition.norm() / 0.5;
    faceFactor = Math::saturate(faceFactor, 0.0, 1.0);

    params.tail<3>() =
      faceFactor * behindBallPosition.normalized() +
      (1.0 - faceFactor) * goalPosition.normalized();
  }

  // Filter parameters to prevent switching directory too quickly
  d_paramFilter += 0.05 * (params - d_paramFilter);
  return d_paramFilter;
}
