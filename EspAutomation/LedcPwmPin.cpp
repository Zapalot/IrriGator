#include "LedcPwmPin.h"
#include "Arduino.h"

uint8_t LedcPwmPin::nextLedcChannel = 0;

void LedcPwmPin::setup(uint8_t pin, uint32_t frequency, uint8_t resolution)
{
    ledcSetup(nextLedcChannel, frequency, resolution); // 12 kHz
    myChannel = nextLedcChannel;
    nextLedcChannel++;
}
void LedcPwmPin::setOutput(uint32_t duty)
{
    ledcWrite(myChannel, duty);
}
uint32_t LedcPwmPin::getOutput()
{
    return ledcRead(myChannel);
}
