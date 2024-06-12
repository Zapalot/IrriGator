#pragma once
#include "Measurements.h"
#include <list>

// implements a circular buffer for a number of measurements
class MeasurementRecordBuffer{
    public:
    void initialize(std::list<AbstractMeasurementChannel*>* channels, size_t nTimePoints);
    ~MeasurementRecordBuffer();
    void measureAllChannels(); // calls 
    void saveRecord(); // reads measurements from the channels in the list supplied at initialization and puts them in the ring buffer
    MeasType getMeasurement(size_t timePointIndex, size_t channelIndex); // time points count backwards from  last update (0) up to nTimePointsMax-1
    timeval getRecordTime(size_t timePointIndex); // time points count backwards from  last update (0) up to getMaxTimePoint-1
    size_t getNumTimePointsInBuffer(); // number of records available
    size_t getNumChannels(){return nChannels;}; // number of records available
    uint32_t getTotalMeasurementCount(){return totalMeasurementCount;}; // how many Records have been saved in total?
    std::list<AbstractMeasurementChannel*>* getChannelList(){return channels;}
    void printContent(bool includeHeader); // print a table of the content to serial
protected:
size_t getLookUpIndex(size_t timePointIndex);
    std::list<AbstractMeasurementChannel*>* channels=nullptr;
    size_t nChannels=0;
    MeasType* measurementBuffer=nullptr;
    timeval* timePointBuffer=nullptr;
    size_t lastTimePointIndex=0; 
    size_t timePointCapacity=0;
    size_t timePointsAvailable=0;
    uint32_t totalMeasurementCount=0;
};