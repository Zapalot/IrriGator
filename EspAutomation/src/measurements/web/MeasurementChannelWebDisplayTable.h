#pragma once

#include "../Measurements.h"
#include "ArduParWeb.h"
#include <vector>
/*
// in the style of ArduAprWeb, this Element will output a single Table Row with the Measurement Channel Name, Description, and current Value
class MeasurementChannelWebDisplayElement: public WebFormAbstractUiElement{
    public:
     void generateHtml(String& outputBuffer); ///< Generates a line of a Table
     void reactToRequest(WebServer& server){}; ///< no reactions to parameters
    AbstractMeasurementChannel* sourceChannel; // data is read from this channel
};
*/
// Creates a Table with one Row per MeasurementChannel, displaying Channel Name, Description, and current Value
class MeasurementChannelWebDisplayTable: public WebFormAbstractUiElement{
    public:
    //void addChannel(AbstractMeasurementChannel* sourceChannel); // create a display and add it to the table
    void generateHtml(String& outputBuffer); ///< Generates a line of a Table
    void reactToRequest(WebServer& server){}; ///< no reactions to parameters
    std::vector<AbstractMeasurementChannel*> channels;
    

};

