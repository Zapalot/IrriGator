#pragma once
#include "AbstractController.h"
#include "AbstractSingleChannelActuator.h"
#include <Arduino.h>
#include "TimeBase.h"
#include <Preferences.h>
#include "ArduPar3.h"


#define  ScheduledOnOffControllerMaxParNameLength  32
class ScheduledOnOffController : public AbstractController
{

    ScheduledOnOffController(const ScheduledOnOffController&)=delete;                 // Prevent copy-construction
    ScheduledOnOffController& operator=(const ScheduledOnOffController&)=delete;      // Prevent assignment
public:
    ScheduledOnOffController(){};
    enum ScheduleTimeBaseUnit{
        ScheduleTimeBaseHours=0,
        ScheduleTimeBaseMinutes=1,
        ScheduleTimeBaseSeconds=2
    };
    void setup(AbstractSingleChannelActuator *actuator, 
            String addressPrefix, String description,
            AbstractTimeBase* timeBase,
               ScheduleTimeBaseUnit timeBaseUnit,
               float defaultCycleStart,
               float defaultCycleLength,
               float defaultOnTime); // will load settings from Preferences
    void update( );                          // will check current time, and adjust actuator output accordingly
    bool getIsOn(){return myActuator->getIntensityFloat()>0.5f;}
    void startBurst(); // start an unscheduled actuator activation
    void stopBurst();// stop an unscheduled actuator activation
private:


    ScheduleTimeBaseUnit timeBaseUnit;
    double timeBaseUnitMultiplier=3600.0*1000000.0;// length of one setting unit in micros
    AbstractTimeBase* timeBase;

    int64_t lastBurstStartTime; 
    // Names for for run-time initialization of parameters
    String cycleStartName;
    String cycleLengthName;
    String onTimeName;

    String scheduleEnabledName;

    String startBurstName;
    String stopBurstName;
    String burstDurationName;



    // for configuraing the regular schedule
    SingleFloatArduPar3 cycleStartPar;
    SingleFloatArduPar3 cycleLengthPar;
    SingleFloatArduPar3 onTimePar;

    SingleBoolArduPar3 scheduleEnabledPar; // if disabled, schedule actuator will always be off

    // used to run the actuator for a defined time out of schedule
    CallbackArduPar3 startBurstPar; 
    CallbackArduPar3 stopBurstPar; 
    SingleFloatArduPar3 burstDurationPar;

    AbstractSingleChannelActuator *myActuator;
};