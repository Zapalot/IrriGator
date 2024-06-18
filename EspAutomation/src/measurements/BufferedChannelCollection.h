#pragma once
#include <vector>
#include <list>
#include "Measurements.h"
#include "IndexedFiFo.h"

//
class BufferedChannelCollection
{
public:
    void setup(std::vector<AbstractMeasurementChannel *> *channels, uint32_t nTimePoints); // create channel buffers
    void pushMeasurementsToBuffer();                                                       // add a snapshot of the current measurements to the buffer
    void pushMeasurementsToBufferInterval(); // checks if minPushIntervalMillis have expired, then calls pushMeasurementsToBuffer
    const std::vector<IndexedFiFo<Measurement>> *getChannelBuffers() const{return &channelBuffers; };                // get a pointer to the buffered channels
    const std::vector<AbstractMeasurementChannel*> *getChannels() const{return &channels; };                // get a pointer to the buffered channels

    uint32_t getNTimePointsInBuffer() const {return channelBuffers[0].getNumEntriesInBuffer();};                                               // return buffered number of timepoints
    bool newChunkIsReady() const {return channelBuffers[0].willWarpOnNextPush();};                                                      //  true once every "buffer capacity" update cycles
    uint32_t minPushIntervalMillis=0; // how much time to wait between buffer saves when using pushMeasurementsToBufferInterval();
protected:
    uint32_t lastPushMillis=0; // last time measurents were saved in the Buffers
    std::vector<IndexedFiFo<Measurement>> channelBuffers;
    std::vector<AbstractMeasurementChannel *> channels;
};