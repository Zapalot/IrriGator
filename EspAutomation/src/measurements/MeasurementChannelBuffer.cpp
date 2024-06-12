#include "MeasurementChannelBuffer.h"

#include "Arduino.h"
void MeasurementChannelBuffer::initialize(AbstractMeasurementChannel *channels, uint32_t nTimePoints)
{
    this->channel = channel;
    timePointCapacity = nTimePoints;
    if (measurementBuffer != nullptr)
        delete[] measurementBuffer;
    measurementBuffer = new Measurement[timePointCapacity];
    lastTimePointIndex = 0;
    totalMeasurementCount = 0;
}
MeasurementChannelBuffer::~MeasurementChannelBuffer()
{
    if (measurementBuffer != nullptr)
        delete[] measurementBuffer;
}
// reads measurements from the channels in the list supplied at initialization and puts them in the ring buffer
void MeasurementChannelBuffer::saveCurMeasToBuffer()
{
    totalMeasurementCount++;
    lastTimePointIndex++;
    if (lastTimePointIndex >= timePointCapacity)
        lastTimePointIndex = 0;
    measurementBuffer[lastTimePointIndex] = channel->getLastMeasurement();
}
// time points count backwards from  last update (0) up to nTimePointsMax-1
Measurement MeasurementChannelBuffer::getMeasurement(uint32_t timePointIndex)
{
    if (timePointIndex < getNumTimePointsInBuffer() )
    {
        return (measurementBuffer[getLookUpIndex(timePointIndex)]);
    }
    else
        return {0,0};
}

uint32_t MeasurementChannelBuffer::getLookUpIndex(uint32_t timePointIndex)
{
    uint32_t lookupIndex = timePointIndex - lastTimePointIndex;
    while(lookupIndex>=timePointCapacity)lookupIndex-=timePointCapacity;
    while(lookupIndex<0)lookupIndex+=timePointCapacity;
    return lookupIndex;
}