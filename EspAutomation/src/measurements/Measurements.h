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
    virtual Measurement getLastMeasurement()const =0;
    virtual MeasType getMin() const =0; //lowest Value that can occur
    virtual MeasType getMax()const =0; //highest Value that can occur
    virtual const char* getShortName()const =0;// some concise human readable identifier ("InletTempertature")
    virtual const char* getDescription()const =0;// some human readable description of arbitrary length 
    virtual const char* getUnit()const =0; // unit of measurement
};
