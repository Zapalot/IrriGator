#pragma once
#include "Arduino.h"
#include "AbstractPwmPin.h"
#include "AbstractSingleChannelActuator.h"
class PulseSustainActuator: public AbstractSingleChannelActuator{
    public:
    enum ActivationState{
    ActivationOff=0,
    ActivationPulse=1,
    ActivationSustain=2
};
    void setup(std::string uniqueName,std::string description, AbstractPwmPin* pin,uint32_t activationPulseLength,int pulseDuty, int sustainDuty,bool initialState);

    void setIntensityBool(bool intensity); // intensity on a int scale, min-max
    bool getIntensityBool(); //intensity on a int scale, min-max

     std::string getUniqueName(){return uniqueName;};
     std::string getDescription(){return description;};
     IntensityType getType(){return AbstractSingleChannelActuator::IntensityBool;};
// state
private:
    ActivationState currentState=ActivationOff;
    uint32_t lastActivationTime; // in "millis()"

    void startPulse();
    void startSustain();
    void turnOff();
    //settings:
    uint32_t activationPulseLength; // time of the initial pulse in ms 
    int pulseDuty=255; // duty cycle on a 0-255 range in the time immediately after activation
    int sustainDuty=255; // duty cycle to keep the valve in open position after the initial pulse
    AbstractPwmPin* pin;
    std::string uniqueName;
    std::string description;
};