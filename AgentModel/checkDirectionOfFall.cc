#include "agentmodel.ih"

void AgentModel::checkDirectionOfFall()
{
    Localizer& loc = SLocalizer::getInstance();
    
    float forceLFootZ = d_footForces[0].z();
    float forceRFootZ = d_footForces[1].z();
    float gyroAbsXY = abs(d_gyro.x()) + abs(d_gyro.y()) ;    

    Vector3d fieldNormal = Math::getUp(loc.getGlobalTransformation());
    
    if (forceLFootZ < 2 && forceRFootZ < 2 &&
        gyroAbsXY > 60.0 / 180.0 * M_PI && fieldNormal.z() < 0.85 && fieldNormal.y() < 0.9) 
    {
  		_debugLevel1(" FALLING!");
      //d_catchFallEnabled = false;
      
      //if front/backwards vector is bigger than sidewaysvector
      if (abs(fieldNormal.y()) > abs(fieldNormal.x()))
      {
    		_debugLevel1("Fall to front: " << (fieldNormal.y() < 0));
        //if fieldNormalX is smaller than 0 the robot is falling forwards
        d_fallDirection = (fieldNormal.y() < 0) ? FALL_TO_FRONT : FALL_TO_BACK;
      }
      else
      {
        _debugLevel1(" Fall to SIDE!");
        d_fallDirection = (fieldNormal.x() > 0) ? FALL_TO_RIGHT : FALL_TO_LEFT;
      }
  }
  else
    d_fallDirection = NOT_FALLING;
}

