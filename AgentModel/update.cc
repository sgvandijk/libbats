#include "agentmodel.ih"

void AgentModel::update()
{
  Cochlea& cochlea = SCochlea::getInstance();

  updatePosture();
  //updateCOM();
  
  
  d_footForceCenters[0] = cochlea.getInfo(Cochlea::iFootLeft).head<3>();
  d_footForceCenters[1] = cochlea.getInfo(Cochlea::iFootRight).head<3>();
  
  d_footForces[0] = cochlea.getInfo(Cochlea::iFootLeft2).head<3>();
  d_footForces[1] = cochlea.getInfo(Cochlea::iFootRight2).head<3>();
  
  d_previousGyro = d_gyro;
  d_gyro = cochlea.getInfo(Cochlea::iTorsoGyro).head<3>() * M_PI / 180.0;

  d_acc = cochlea.getInfo(Cochlea::iAcc).head<3>();
  
  _debugLevel4("gyro: " << d_gyro);
  checkDirectionOfFall();
}
