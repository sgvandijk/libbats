#include "localizer.ih"

Localizer::ObjectVector Localizer::getAliveObjects(std::vector<shared_ptr<DynamicObjectInfo>> objects, std::function<bool(shared_ptr<DynamicObjectInfo>)> pred)
{
  ObjectVector filtered = ObjectVector();
  remove_copy_if(objects.begin(), objects.end(),
		 filtered.begin(),
		 [&pred] (shared_ptr<DynamicObjectInfo> object) {
		   return !object->isAlive || (pred && !pred(object));
		 });
  return filtered;
};
