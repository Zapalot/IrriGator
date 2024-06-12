#pragma once
#include <time.h>
#include "Arduino.h"
class AbstractTimeBase
{
public:
    virtual int64_t getUnixTimeMicros() = 0;
    virtual int64_t getLocalUnixTimeMicros() = 0;
    virtual int64_t getTimeZoneOffsetMicros() = 0; // offset from UTC, in seconds (no daylight saving)
    virtual double getHoursOfTheDay() = 0;
};

class SystemTimeBase : public AbstractTimeBase
{
public:
    int64_t getUnixTimeMicros();
    int64_t getLocalUnixTimeMicros();
    int64_t getTimeZoneOffsetMicros();

    double getHoursOfTheDay();
    int64_t speedUpFactor = 1;
    int64_t startTime = 0;
    int64_t timeZoneOffset = 0;
    void initStartTime();
};