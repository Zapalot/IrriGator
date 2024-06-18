#include "BufferedMeasurementLogger.h"

// Opens File, writes a Header with channel descriptions
void BufferedMeasurementLogger::setup(BufferedChannelCollection *channelCollection, AbstractLogTarget *logTarget)
{
    this->channelCollection = channelCollection;
    this->logTarget = logTarget;
    logTarget->startOutput();
    // write a header
    bool firstChannel = true;
    const std::vector<AbstractMeasurementChannel *> *channels = channelCollection->getChannels();
    Print *output = logTarget->getPrintObj();
    for (auto &channel : *channels)
    {
        if (!firstChannel)
        {
            output->print("\t");
            firstChannel = true;
        }
        output->print(channel->getShortName());
    }
    output->print("\n");
    logTarget->stopOutput();
}
// query measurements from Buffer and write them to file
void BufferedMeasurementLogger::update()
{
    if (channelCollection->newChunkIsReady())
    {
        logTarget->startOutput();
        // write all the accumulated data in the buffers
        bool firstChannel = true;
        auto channelBuffers = channelCollection->getChannelBuffers();
        Print *output = logTarget->getPrintObj();
        for (uint32_t index = 0; index < channelCollection->getNTimePointsInBuffer(); index++)
        {
            for (auto &channel : *channelBuffers)
            {
                if (!firstChannel)
                {
                    output->print("\t");
                    firstChannel = true;
                }
                output->print(channel.getEntry(index).value);
            }
            output->print("\n");
        }
        logTarget->stopOutput();
    }
}
