#include "SdCardMeasurementLogger.h"

// close file
void SdCardMeasurementLogger::setup(const char *filename, int sdCsPin, MeasurementRecordBuffer *recordBuffer)
{
    this->recordBuffer = recordBuffer;
    // Initialize SD card
    if (!SD.begin(sdCsPin))
    {
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return;
    }

    // If the data.txt file doesn't exist
    // Create a file on the SD card and write the data labels
    File file = SD.open(filename);
    if (!file)
    {
        Serial.println("File doens't exist");
        Serial.printf("Creating file: %s\n", filename);

        file = SD.open(filename, FILE_WRITE);
        if (!file)
        {
            Serial.println("Failed to open file for writing");
            return;
        }
        // create header
        bool first = true;
        for (auto curChannel : *recordBuffer->getChannelList())
        {
            if (!first)
                file.print("\t");
            first = false;
            file.print(curChannel->getShortName());
        }
        if (file.print("\n"))
        {
            Serial.println("File initialized");
        }
        else
        {
            Serial.println("Write failed");
        }
        file.close();
    }
    else
    {
        Serial.println("Logfile already exists - appending");
    }
    file.close();
}
// query measurements and write them to file
void SdCardMeasurementLogger::update()
{
    uint32_t curMeasCount = recordBuffer->getTotalMeasurementCount();
    uint32_t measCountSinceLastWrite = curMeasCount - measCountOnLastWrite;

    if (measCountSinceLastWrite >= recordsPerSdWriteInterval)
    {
        if (recordBuffer->getNumTimePointsInBuffer() < measCountSinceLastWrite)
        {
            Serial.println("Warning! Buffer doesn't contain all measurements since last write!");
            measCountSinceLastWrite = recordBuffer->getNumTimePointsInBuffer();
        }
        measCountOnLastWrite = curMeasCount;
        File file = SD.open(filename, FILE_APPEND);
        if (!file)
        {
            Serial.println("Failed to open file for appending");
            return;
        }
        for (uint32_t i = 0; i < measCountSinceLastWrite; i++)
        {
            for (size_t channelIndex = 0; channelIndex < recordBuffer->getNumChannels(); channelIndex++)
            {
                if (channelIndex != 0)
                {
                    file.print("\t");
                }
            }
            file.print("\n");
        }
        file.close();
    }
}
