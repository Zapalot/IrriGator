#include "BufferedChannelCollection.h"
#include "Arduino.h"
// create channel buffers
void BufferedChannelCollection::setup(std::vector<AbstractMeasurementChannel *> *channels, uint32_t nTimePoints)
{
    this->channels = *channels;              // make a copy of the channel vecor
    channelBuffers.resize(channels->size()); // create buffers for date from all the channels
    for (int i = 0; i < channels->size(); i++)
    {
        channelBuffers[i].initialize(nTimePoints);
    }
}
// add a snapshot of the current measurements to the buffer
void BufferedChannelCollection::pushMeasurementsToBuffer()
{
    for (int i = 0; i < channelBuffers.size(); i++)
    {
        channelBuffers[i].pushEntry(channels[i]->getLastMeasurement());
    }
    lastPushMillis = millis();
}
// checks if minPushIntervalMillis have expired, then calls pushMeasurementsToBuffer
void BufferedChannelCollection::pushMeasurementsToBufferInterval()
{
    if (millis() - lastPushMillis > minPushIntervalMillis)
        pushMeasurementsToBuffer();
}
