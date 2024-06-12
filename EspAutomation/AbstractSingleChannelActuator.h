#pragma once
#include <string>
class AbstractSingleChannelActuator{
    public:
    enum IntensityType{
        IntensityBool=0,
        IntensityInt=1,
        IntensityFloat=2
    };
// basic idea: every actuator can be set using ints or even floats, they will just be cast down to the ints ot bools if not implemented

    virtual void setIntensityBool(bool intensity)=0; // intensity on a int scale, min-max
    virtual bool getIntensityBool()=0; //intensity on a int scale, min-max

    virtual void setIntensityInt(int intensity){setIntensityBool(intensity>(getMaxInt()+getMinInt())/2);}; // intensity on a int scale, min-max
    virtual int getIntensityInt(){getIntensityBool()?getMaxInt():getMinInt();}; //intensity on a int scale, min-max

    virtual void setIntensityFloat(float intensity){setIntensityInt(intensity);}; // intensity on a float scale, min-max
    virtual float getIntensityFloat(){return (float)getIntensityInt(); }; //intensity on a float scale, min-max


    virtual int32_t getMinInt(){return 0;};
    virtual int32_t getMaxInt(){return 1;};
    virtual float getMinFloat(){return getMinInt();};
    virtual float getMaxFloat(){return getMaxInt();};
    virtual std::string getUniqueName()=0;
    virtual std::string getDescription()=0;
    virtual IntensityType getType()=0;
};