#include "robovizdebugger.ih"

void RoboVizDebugger::drawTransformationAxes(Eigen::Matrix4d const& matrix, float thickness, float axisLength, const std::string* setName)
{
  Vector3d origin = (matrix * Vector4d(0,0,0,0)).head<3>();
  Vector3d unitX  = (matrix * Vector4d(1,0,0,0)).head<3>() * axisLength;
  Vector3d unitY  = (matrix * Vector4d(0,1,0,0)).head<3>() * axisLength;
  Vector3d unitZ  = (matrix * Vector4d(0,0,1,0)).head<3>() * axisLength;
  
  drawLine(origin, unitX, thickness, Vector4d(1,0,0,1), setName);
  drawLine(origin, unitY, thickness, Vector4d(0,1,0,1), setName);
  drawLine(origin, unitZ, thickness, Vector4d(0,0,1,1), setName);

  unitX.z() = unitY.z() = unitZ.z() = 0;
  
  drawLine(origin, unitX, thickness, Vector4d(0.5,0.0,0.0,1), setName);
  drawLine(origin, unitY, thickness, Vector4d(0.0,0.5,0.0,1), setName);
  drawLine(origin, unitZ, thickness, Vector4d(0.0,0.0,0.5,1), setName);
}
