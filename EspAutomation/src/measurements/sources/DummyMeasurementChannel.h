#pragma once    
#include "../Measurements.h"

class DummyMeasurementChannel : public AbstractMeasurementChannel
{
public:
    void setup(
        );
    void update(){lastValue=lastValue>99.9?0.0:lastValue+1.0;}
    Measurement getLastMeasurement() const {
        Measurement retVal;
        retVal.value=lastValue;
        gettimeofday(&retVal.measTime, NULL);
        return retVal;
        };
    MeasType getMin()const {return 0;};            // lowest Value that can occur
    MeasType getMax()const {return 100;};            // highest Value that can occur
    const char *getShortName()const {return "dummy";};   // some concise human readable identifier ("InletTempertature")
    const char *getDescription()const {return "Update call count";}; // some human readable description of arbitrary length
    const char *getUnit()const {return "calls";};        // unit of measurement
    MeasType lastValue=0;
};