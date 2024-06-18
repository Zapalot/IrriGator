#include "SdCardLogTarget.h"
// Initializes SD, creates File, closes it again
void SdCardLogTarget::setup(const char *filename, int sdCsPin)
{
    this-> filename=filename;
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
    file = SD.open(filename);
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
    }
    file.close();
            Serial.println("Log File already exists!");
}

// open the output, potentially bringing it into an unsave state for power down
void SdCardLogTarget::startOutput()
{
    if (!file)
    {
        file = SD.open(filename, FILE_APPEND);
        if (!file)
        {
            Serial.println("Failed to open file for appending");
            return;
        }
    }
}
// write all pending changes and bring file into a save state for power down
void SdCardLogTarget::stopOutput() {
    file.close();
}
// get a print object that can be used to write to the file
Print *SdCardLogTarget::getPrintObj() {return &file;}