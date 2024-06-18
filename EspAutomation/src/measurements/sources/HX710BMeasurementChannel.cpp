#include "HX710BMeasurementChannel.h"

void HX710BMeasurementChannel::setup(
    HX710Array *interface, int sensorIndex,
    const char *shortName, const char *description, const char *unit,
    float offset, float scale,
    float min, float max)
{
    myInterface = interface;
    mySensorIndex = sensorIndex;
    myShortName = shortName;
    myDescription = description;
    myUnit = unit;
    myOffset = offset;
    myScale = scale;
    myMax = max;
    myMin = min;
}
void HX710BMeasurementChannel::update()
{
    myInterface->readAndSelectNextData(25);
}
Measurement HX710BMeasurementChannel::getLastMeasurement()
{
    Measurement retVal;
    int32_t rawData = myInterface->getLastDifferentialInput(mySensorIndex);
    retVal.value = rawData * myScale + myOffset;
    gettimeofday(&retVal.measTime, NULL);
    return retVal;
}
MeasType HX710BMeasurementChannel::getMin() { return myMin; }                    // lowest Value that can occur
MeasType HX710BMeasurementChannel::getMax() { return myMax; }                    // highest Value that can occur
const char *HX710BMeasurementChannel::getShortName() { return myShortName; }     // some concise human readable identifier ("InletTempertature")
const char *HX710BMeasurementChannel::getDescription() { return myDescription; } // some human readable description of arbitrary length
const char *HX710BMeasurementChannel::getUnit() { return myUnit; }               // unit of measurement