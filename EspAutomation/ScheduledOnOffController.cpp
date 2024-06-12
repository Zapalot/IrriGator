#include "ScheduledOnOffController.h"
void ScheduledOnOffController::setup(AbstractSingleChannelActuator *actuator,
                                     String addressPrefix, String description,
                                     AbstractTimeBase* timeBase,
                                     ScheduleTimeBaseUnit timeBaseUnit,
                                     float defaultCycleStart,
                                     float defaultCycleLength,
                                     float defaultOnTime)
{

    // buint addresses for parameters
    cycleStartName=addressPrefix+"/cycleStart";
    cycleLengthName=addressPrefix+"/cycleLength";
    onTimeName=addressPrefix+"/onTime";
    scheduleEnabledName=addressPrefix+"/enableSchedule";

    burstDurationName=addressPrefix+"/burstDuration";
   startBurstName=addressPrefix+"/startBurst";
   stopBurstName=addressPrefix+"/stopBurst";
    float maxCycleLength;
    switch (    timeBaseUnit)
    {
    case ScheduleTimeBaseHours:
        maxCycleLength=24*7;
        break;
            case ScheduleTimeBaseMinutes:
        maxCycleLength=24*60;
        break;
            case ScheduleTimeBaseSeconds:
        maxCycleLength=60*60;
        break;
    default:
        break;
    }

    cycleStartPar.setup(cycleStartName.c_str(),"Cycle Start (New Year 0:00 = 0)",0,maxCycleLength);
    cycleLengthPar.setup(cycleLengthName.c_str(),"Cycle Length (hours)",0,maxCycleLength);
    onTimePar.setup(onTimeName.c_str(),"On Time (hours)",0,maxCycleLength);
    scheduleEnabledPar.setup(scheduleEnabledName.c_str(),"Enable Schedule");

    burstDurationPar.setup(burstDurationName.c_str(),"Burst Duration",0,24);
    startBurstPar.setup(startBurstName.c_str(),"Start Burst!",std::bind(&ScheduledOnOffController::startBurst, this));
    stopBurstPar.setup(stopBurstName.c_str(),"Stop Burst!",std::bind(&ScheduledOnOffController::stopBurst, this));

    this->timeBaseUnit=timeBaseUnit;
    this->timeBase=timeBase;
    this->myActuator=actuator;

    lastBurstStartTime=-1000000ll*3600ll*24ll*356ll; // make sure the burst is already over...
}

void ScheduledOnOffController::update( )
{
    int64_t curTime=timeBase->getLocalUnixTimeMicros();
    int64_t microsSinceBurstStart=(curTime-lastBurstStartTime);
    bool burstIsActive=microsSinceBurstStart<(int64_t)(burstDurationPar.getValue()*timeBaseUnitMultiplier);

   // onTimePar.setMax(cycleLengthPar.getValue());

    int64_t cycleStartMicros=(int64_t)(cycleStartPar.getValue()*timeBaseUnitMultiplier);
    int64_t cycleLengthMicros=(int64_t)(cycleLengthPar.getValue()*timeBaseUnitMultiplier);
    int64_t onTimeMicros=(int64_t)(onTimePar.getValue()*timeBaseUnitMultiplier);

    int64_t microsInInterval=(int64_t)(curTime-cycleStartMicros)%max((int64_t)cycleLengthMicros,(int64_t)1);
/*
    Serial.print("curTime=");
    Serial.println(curTime);

    Serial.print("timeBaseUnitMultiplier=");
    Serial.println(timeBaseUnitMultiplier);

    Serial.print("cycleLengthMicros=");
    Serial.println(cycleStartMicros);

    Serial.print("onTimeMicros=");
    Serial.println(onTimeMicros);
*/
    Serial.print("microsSinceBurstStart=");
    Serial.println(microsSinceBurstStart);

    Serial.print("burstDuration");
    Serial.println(burstDurationPar.getValue()*timeBaseUnitMultiplier);

    Serial.print("microsInInterval=");
    Serial.println(microsInInterval);

    bool isOn=(microsInInterval<onTimeMicros)&&scheduleEnabledPar.getValue()||burstIsActive;
    myActuator->setIntensityBool(isOn);
}
// start an unscheduled actuator activation
void ScheduledOnOffController::startBurst(){
    lastBurstStartTime=timeBase->getLocalUnixTimeMicros();
} 
// stop an unscheduled actuator activation
void ScheduledOnOffController::stopBurst(){
    lastBurstStartTime=-1000000ll*3600ll*24ll*356ll; // make sure the burst is already over...
}