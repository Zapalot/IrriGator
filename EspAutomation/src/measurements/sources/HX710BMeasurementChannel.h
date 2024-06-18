#pragma once
#include "HX710Array.h"
#include "../Measurements.h"
class HX710BSensorChannelArray
{
};
class HX710BMeasurementChannel : public AbstractMeasurementChannel
{
public:
    void setup(
        HX710Array *interface, int sensorIndex,
        const char *shortName, const char *description, const char *unit,
        float offset, float scale,
        float min, float max
        );
    void update();
    Measurement getLastMeasurement();
    MeasType getMin();            // lowest Value that can occur
    MeasType getMax();            // highest Value that can occur
    const char *getShortName();   // some concise human readable identifier ("InletTempertature")
    const char *getDescription(); // some human readable description of arbitrary length
    const char *getUnit();        // unit of measurement
protected:
    const char *myShortName;
    const char *myDescription;
    const char *myUnit;

    float myOffset;
    float myScale;

    float myMin;
    float myMax;
    HX710Array *myInterface;
    int mySensorIndex;
};
