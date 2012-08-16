#include "localizer.ih"

Localizer::ObjectVector Localizer::getFilteredObjects(std::vector<shared_ptr<ObjectInfo>> objects, std::function<bool(shared_ptr<ObjectInfo>)> pred)
{
  ObjectVector filtered = Localizer::ObjectVector();
  for (shared_ptr<ObjectInfo> object : objects)
  {
    if (bool(pred(object)))
      filtered.push_back(object);
  }
  return filtered;
}