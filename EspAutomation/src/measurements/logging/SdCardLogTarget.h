#pragma once
#include "Arduino.h"
#include "LogTargets.h"
#include "SD.h"
// the logger class provides an Arduino "Print" object that can be used to write data, and adds methods for bringing the output device to a save state between writes
class SdCardLogTarget: public AbstractLogTarget{
public:
   void setup(const char* filename, int sdCsPin); // Initializes SD, creates File, closes it again
     void startOutput(); // open the output, potentially bringing it into an unsave state for power down
     void stopOutput();   // write all pending changes and bring file into a save state for power down
     Print* getPrintObj(); // get a print object that can be used to write to the file
        const char* getFilename(){return filename;}; 
    protected:
    File file; // kept open in append mode between calls of startOutput and endOutput
    const char* filename; 
};