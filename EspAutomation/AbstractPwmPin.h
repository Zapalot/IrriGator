#pragma once
#include <Arduino.h>
//a hardware abstraction to support PWM pins on different Platforms
class AbstractPwmPin{
    public:
    virtual void setOutput(uint32_t duty)=0;
    virtual uint32_t getOutput()=0;
};
