#pragma once
#include "Arduino.h"
// the logger class provides an Arduino "Print" object that can be used to write data, and adds methods for bringing the output device to a save state between writes
class AbstractLogTarget{
public:
    virtual void startOutput(){}; // open the output, potentially bringing it into an unsave state for power down
    virtual void stopOutput(){};   // write all pending changes and bring file into a save state for power down
    virtual Print* getPrintObj()=0; // get a print object that can be used to write to the file
};

/*
//a log target that does nothing
class NullLogTraget:public AbstractLogTarget{
     void startOutput(){}; // open the output, potentially bringing it into an unsave state for power down
     void stopOutput(){};   // write all pending changes and bring file into a save state for power down
     Print* getPrintObj(){}; // get a print object that can be used to write to the file
};
*/

//a log target that prints to the Serial
class SerialLogTarget:public AbstractLogTarget{
     void startOutput(){}; // open the output, potentially bringing it into an unsave state for power down
     void stopOutput(){};   // write all pending changes and bring file into a save state for power down
     Print* getPrintObj(){return &Serial;}; // get a print object that can be used to write to the file
};
