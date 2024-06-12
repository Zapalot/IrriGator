#pragma once    
#include "Measurements.h"

class DummyMeasurementChannel : public AbstractMeasurementChannel
{
public:
    void setup(
        );
    void update(){lastValue+=1.0;}
    Measurement getLastMeasurement(){
        Measurement retVal;
        retVal.value=lastValue;
        gettimeofday(&retVal.measTime, NULL);
        return retVal;
        };
    MeasType getMin(){return 0;};            // lowest Value that can occur
    MeasType getMax(){return 100;};            // highest Value that can occur
    const char *getShortName(){return "dummy";};   // some concise human readable identifier ("InletTempertature")
    const char *getDescription(){return "Update call count";}; // some human readable description of arbitrary length
    const char *getUnit(){return "calls";};        // unit of measurement
    MeasType lastValue=0;
};