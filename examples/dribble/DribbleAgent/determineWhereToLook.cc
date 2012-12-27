#include "dribbleagent.ih"

Vector2d DribbleAgent::determineWhereToLook()
{
  Localizer& localizer = SLocalizer::getInstance();
  AgentModel& agentModel = SAgentModel::getInstance();

  if (localizer.ball->isAlive)
  {
    // We have a reliable idea where the ball is. Get the
    // position. Pass 'true' to set the z-coordinate to 0
    Vector3d ballPosition = localizer.ball->getPositionLocal(true);

    // Determine ball position relative to head, in agent frame
    Vector3d ballPositionAF =
      localizer.getLocalTransformation().inverse() * ballPosition;
    Vector3d headToBallAF =
      ballPositionAF - agentModel.getBodyPartPosition(Types::HEAD);

    // Determine polar coordinates of ball relative to head, in agent
    // frame
    Vector3d ballPositionPolar = Math::cartesianToPolar(headToBallAF);

    // The angles are the last two of the polar coordinates
    return ballPositionPolar.tail<2>();
  }
  else
  {
    // We don't know where the ball is, look around and up and down
    // until we do
    Clock& clock = SClock::getInstance();

    // Periods of left-right and up-down motion are 2 and .5 seconds,
    // respectively
    double periodZ = 2.0;
    double periodX = 0.5;

    // Determine looking angles, making sure we only look down
    double phaseZ = clock.getTime() / periodZ * 2 * M_PI;
    double phaseX = clock.getTime() / periodX * 2 * M_PI;

    double angleZ = Math::degToRad(120.0 * sin(phaseZ));
    double angleX = Math::degToRad(45.0 * .5 * sin(phaseX)) - .5;

    return Vector2d(angleZ, angleX);
  }
}
