#include "robovizdebugger.ih"

/*
void RoboVizDebugger::plot(std::string const& curve, float value)
{
  if (d_curves.find(curve) == d_curves.end())
    d_curves[curve] = Curve();
  
  Curve& c = d_curves[curve];
  
  c.data.push_back(value);
  c.maxX = d_curves[curve].data.size();
  if (c.maxX > 1000)
   c.minX = c.maxX - 1000;
  
  c.minY = c.maxY = c.data[c.minX];
  for (unsigned x = c.minX + 1; x < c.maxX; ++x)
  {
    if (c.maxY < c.data[x])
      c.maxY = c.data[x];
    if (c.minY > c.data[x])
      c.minY = c.data[x];
  }
}
*/
