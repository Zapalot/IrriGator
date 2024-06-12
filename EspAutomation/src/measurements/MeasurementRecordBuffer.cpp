#include "MeasurementRecordBuffer.h"
#include "Arduino.h"
void MeasurementRecordBuffer::initialize(std::list<AbstractMeasurementChannel *> *channels, size_t nTimePoints)
{
    this->channels = channels;
    this->nChannels = channels->size();
    this->timePointCapacity = nTimePoints;
    if (measurementBuffer != nullptr)
        delete[] measurementBuffer;
    measurementBuffer = new MeasType[nChannels * timePointCapacity];
    if (timePointBuffer != nullptr)
        delete[] timePointBuffer;
    timePointBuffer = new timeval[timePointCapacity];
    lastTimePointIndex = 0;
    timePointsAvailable = 0;
}
MeasurementRecordBuffer::~MeasurementRecordBuffer()
{
    if (measurementBuffer != nullptr)
        delete[] measurementBuffer;
    if (timePointBuffer != nullptr)
        delete[] timePointBuffer;
}
// reads measurements from the channels in the list supplied at initialization and puts them in the ring buffer
void MeasurementRecordBuffer::saveRecord()
{
    if (timePointsAvailable < timePointCapacity)
        timePointsAvailable++;
    lastTimePointIndex++;
    if (lastTimePointIndex >= timePointCapacity)
        lastTimePointIndex = 0;
    int channelIndex = 0;
    for (auto curChannel : *channels)
    {
        measurementBuffer[lastTimePointIndex * nChannels + channelIndex] = curChannel->getLastMeasurement().value;
        channelIndex++;
    }
}
// time points count backwards from  last update (0) up to nTimePointsMax-1
MeasType MeasurementRecordBuffer::getMeasurement(size_t timePointIndex, size_t channelIndex)
{
    if (timePointIndex < timePointsAvailable && channelIndex < nChannels)
    {
        return (measurementBuffer[getLookUpIndex(timePointIndex) * nChannels + channelIndex]);
    }
    else
        return 0;
}
timeval MeasurementRecordBuffer::getRecordTime(size_t timePointIndex)
{
    if (timePointIndex < timePointsAvailable)
    {
        return (timePointBuffer[getLookUpIndex(timePointIndex) * nChannels]);
    }
    else
        return {0, 0};
}

// time points count backwards from  last update (0) up to getNumTimePointsInBuffer-1
size_t MeasurementRecordBuffer::getNumTimePointsInBuffer()
{
    return timePointsAvailable;
} // number of records available

size_t MeasurementRecordBuffer::getLookUpIndex(size_t timePointIndex)
{
    size_t lookupIndex = timePointIndex + lastTimePointIndex;
    lookupIndex %= timePointCapacity;
    return lookupIndex;
}
// print a table of the content to serial
void MeasurementRecordBuffer::printContent(bool includeHeader){


}