#pragma once
#include "Measurements.h"

class MeasurementChannelBuffer
{
public:
    void initialize(AbstractMeasurementChannel *channels, uint32_t nTimePoints);
    ~MeasurementChannelBuffer();
    void saveCurMeasToBuffer();  // reads measurement from the channel provided at initialization and puts it in the ring buffer
    Measurement getMeasurement(uint32_t timePointIndex);                   // time points count backwards from  last update (0) up to nTimePointsMax-1
    uint32_t getNumTimePointsInBuffer(){return min(totalMeasurementCount,timePointCapacity);};                                   // number of timepoints available
    uint32_t getTotalMeasurementCount() { return totalMeasurementCount; }; // how many Records have been saved in total?
    AbstractMeasurementChannel *getChannel() { return channel; }

protected:
    uint32_t getLookUpIndex(uint32_t timePointIndex);
    AbstractMeasurementChannel *channel = nullptr;
    Measurement *measurementBuffer = nullptr;
    uint32_t lastTimePointIndex = 0;
    uint32_t timePointCapacity = 0;
    uint32_t totalMeasurementCount = 0;
};