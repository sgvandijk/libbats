#include "cochlea.ih"

vector<VisibleLineJunction> Cochlea::getLineJunctions(vector<VisibleLine> lines, const double thresholdDistance) const
{
  vector<VisibleLineJunction> results;
  
  if (lines.size() < 2)
    return results;
  
  for (unsigned index1 = 0; index1 < lines.size() - 1; index1++)
  {
    for (unsigned index2 = index1 + 1; index2 < lines.size(); index2++)
    {
      auto line1 = lines[index1];
      auto line2 = lines[index2];
      
      if ((line1.end1Cartesian - line2.end1Cartesian).norm() < thresholdDistance)
        results.push_back(VisibleLineJunction(line1, line2, line1.end1Polar, line2.end1Polar));
      else if ((line1.end1Cartesian - line2.end2Cartesian).norm() < thresholdDistance)
        results.push_back(VisibleLineJunction(line1, line2, line1.end1Polar, line2.end2Polar));
      else if ((line1.end2Cartesian - line2.end2Cartesian).norm() < thresholdDistance)
        results.push_back(VisibleLineJunction(line1, line2, line1.end2Polar, line2.end2Polar));
      else if ((line1.end2Cartesian - line2.end1Cartesian).norm() < thresholdDistance)
        results.push_back(VisibleLineJunction(line1, line2, line2.end1Polar, line2.end1Polar));
    }
  }
  
  return results;
}