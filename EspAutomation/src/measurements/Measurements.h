#pragma once
#include <time.h>
#define MeasType double

// represents a single Measurement
class Measurement{
    public:
    MeasType value;
    timeval measTime; ///<time when the measurement was carried out
};


class AbstractMeasurementChannel{
    public:
    virtual Measurement getLastMeasurement()=0;
    virtual MeasType getMin()=0; //lowest Value that can occur
    virtual MeasType getMax()=0; //highest Value that can occur
    virtual const char* getShortName()=0;// some concise human readable identifier ("InletTempertature")
    virtual const char* getDescription()=0;// some human readable description of arbitrary length 
    virtual const char* getUnit()=0; // unit of measurement
};
