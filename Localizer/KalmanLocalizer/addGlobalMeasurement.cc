#include "kalmanlocalizer.ih"

void KalmanLocalizer::addGlobalMeasurement(rf<DynamicObjectInfo> dynamicObject, rf<Distribution> measurement)
{
  if (!dynamicObject || !measurement)
  {
    cout << "(KalmanLocalizer::addGlobalMeasurement) Null measurement" << endl;
    return;
  }

  dynamicObject->posVelGlobal->update(measurement);
  dynamicObject->isAlive = true;
}
