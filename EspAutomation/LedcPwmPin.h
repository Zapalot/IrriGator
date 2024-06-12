#pragma once
#include "AbstractPwmPin.h"
// wraps Ledc PWM control for an output pin
class LedcPwmPin :public AbstractPwmPin{
public:
 void setup(uint8_t pin,uint32_t frequency, uint8_t resolution);
 void setOutput(uint32_t duty);
 uint32_t getOutput();
protected:
    int myPin;
    int myChannel;
    int currentOutput;
    static uint8_t nextLedcChannel; // next free channel 
};