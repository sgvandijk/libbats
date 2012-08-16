#include "kalmanlocalizer.ih"

void KalmanLocalizer::addGlobalMeasurement(shared_ptr<DynamicObjectInfo> dynamicObject, shared_ptr<Distribution> measurement)
{
  if (!dynamicObject || !measurement)
  {
    cout << "(KalmanLocalizer::addGlobalMeasurement) Null measurement" << endl;
    return;
  }

  dynamicObject->posVelGlobal->update(measurement);
  dynamicObject->isAlive = true;
}
