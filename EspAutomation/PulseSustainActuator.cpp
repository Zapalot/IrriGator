#include "PulseSustainActuator.h"

void PulseSustainActuator::setup(std::string uniqueName,std::string description, AbstractPwmPin *pin, uint32_t activationPulseLength, int pulseDuty, int sustainDuty, bool initialState)
{

    this->uniqueName=uniqueName;
    this->description=description; 
    this->pin = pin;
    this->activationPulseLength = activationPulseLength;
    this->pulseDuty = pulseDuty;
    this->sustainDuty = sustainDuty;
    setIntensityBool(initialState);
}
void PulseSustainActuator::setIntensityBool(bool on)
{
    if (on)
    {
        if (currentState == ActivationOff)
        {
            startPulse();
            delay(activationPulseLength);
            startSustain();
        }
    }
    else
    {
        turnOff();
    }
}
bool PulseSustainActuator::getIntensityBool()
{
    return currentState != ActivationOff;
}

void PulseSustainActuator::startPulse()
{
    currentState = ActivationPulse;
    pin->setOutput(pulseDuty);
    lastActivationTime = millis();
}
void PulseSustainActuator::startSustain()
{
    currentState = ActivationSustain;
    pin->setOutput(sustainDuty);
}
void PulseSustainActuator::turnOff()
{
    currentState = ActivationOff;
    pin->setOutput(0);
}