#include "localizer.ih"

Localizer::ObjectVector Localizer::getFilteredObjects(std::vector<rf<ObjectInfo>> objects, std::function<bool(rf<ObjectInfo>)> pred)
{
  ObjectVector filtered = Localizer::ObjectVector();
  for (rf<ObjectInfo> object : objects)
  {
    if (bool(pred(object)))
      filtered.push_back(object);
  }
  return filtered;
}