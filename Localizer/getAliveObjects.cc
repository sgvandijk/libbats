#include "localizer.ih"

Localizer::ObjectVector Localizer::getAliveObjects(std::vector<shared_ptr<DynamicObjectInfo>> objects, std::function<bool(shared_ptr<DynamicObjectInfo>)> pred)
{
  ObjectVector filtered = ObjectVector();
  for (shared_ptr<DynamicObjectInfo> object : objects)
  {
    if (!object->isAlive)
      continue;
    if (!pred || bool(pred(object)))
      filtered.push_back(object);
  }
  return filtered;
};