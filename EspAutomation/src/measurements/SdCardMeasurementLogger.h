#pragma once
#include <list>
#include <vector>
#include "MeasurementRecordBuffer.h"
#include <time.h>
#include "SD.h"


class SdCardMeasurementLogger{
public:
    void setup(const char* filename, int sdCsPin, MeasurementRecordBuffer* recordBuffer); // Opens File, writes a Header with channel descriptions 
    void update(); // query measurements from Buffer and write them to file
        int recordsPerSdWriteInterval=1;
protected:
void writeRecord();
    const char* filename;
    MeasurementRecordBuffer* recordBuffer=nullptr;

    uint32_t measCountOnLastWrite=0;

};