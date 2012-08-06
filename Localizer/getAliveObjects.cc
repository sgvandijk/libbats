#include "localizer.ih"

Localizer::ObjectVector Localizer::getAliveObjects(std::vector<rf<DynamicObjectInfo>> objects, std::function<bool(rf<DynamicObjectInfo>)> pred)
{
  ObjectVector filtered = ObjectVector();
  for (rf<DynamicObjectInfo> object : objects)
  {
    if (!object->isAlive)
      continue;
    if (!pred || bool(pred(object)))
      filtered.push_back(object);
  }
  return filtered;
};