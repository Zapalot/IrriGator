#pragma once
#include <list>
#include <vector>
#include "../Measurements.h"
#include "../BufferedChannelCollection.h"
#include "LogTargets.h"


class BufferedMeasurementLogger{
public:
    void setup(BufferedChannelCollection* channelCollection, AbstractLogTarget* logTarget ); // Opens File, writes a Header with channel descriptions 
    void update(); // query measurements from Buffer and write them to file
protected:
void writeRecord();
void writeHeader();
BufferedChannelCollection* channelCollection=nullptr;
 AbstractLogTarget* logTarget=nullptr;

};