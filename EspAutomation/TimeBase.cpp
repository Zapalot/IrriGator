#include "TimeBase.h"
#include <time.h>

double SystemTimeBase::getHoursOfTheDay() {
    // get current time from RTC

    int64_t now=getUnixTimeMicros();
    int64_t secondsOfTheDay=((now+timeZoneOffset)/1000000l)%(int64_t)(24l*3600l);
    double hoursOfDay=(double)secondsOfTheDay*(1.0/3600.0);
    return hoursOfDay;
}


int64_t SystemTimeBase::getUnixTimeMicros() {

    if(startTime==0){
        initStartTime();
    }
    timeval now;
    gettimeofday(&now,NULL);
    int64_t curMicros=now.tv_sec*1000000l+now.tv_usec;

    // potentially speed up time since start for debug purposes
    curMicros-=startTime;
    curMicros*=speedUpFactor;
    curMicros+=startTime;
    return curMicros;
}
int64_t SystemTimeBase::getLocalUnixTimeMicros(){
     int64_t curMicros=getUnixTimeMicros();
     return curMicros+timeZoneOffset;
}
int64_t SystemTimeBase::getTimeZoneOffsetMicros(){
 return timeZoneOffset;
}

void SystemTimeBase::initStartTime(){
    timeval now;
    gettimeofday(&now,NULL);
    int64_t curMicros=now.tv_sec*1000000l+now.tv_usec;
    startTime=curMicros;
}