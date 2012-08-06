#include "robovizdebugger.ih"

void RoboVizDebugger::drawProgressCircle(Eigen::Vector3d const& center, double radius, double progress, Vector4d color, const std::string* setName)
{
  drawCircle(center.x(), center.y(), radius, 1, color[0], color[1], color[2], setName);
  
  if (progress <= 0)
    return;
  
  if (progress > 1)
    progress = 1;

  double finalAngle = 2*M_PI * progress;

  std::list<Vector3d> vertices;
  vertices.push_back(Vector3d(center.x(), center.y(), 0));
  int parts = (15.0 * progress) + 10;
  for (int i=0; i<parts; i++)
  {
    double angle = i/(double)parts * finalAngle;
    Vector3d point = Vector3d(
      center.x() + sin(angle)*radius,
      center.y() + cos(angle)*radius,
      0);
    vertices.push_back(point);
  }
  vertices.push_back(Vector3d(center.x(), center.y(), 0));

  drawPolygon(vertices, color, setName);
}