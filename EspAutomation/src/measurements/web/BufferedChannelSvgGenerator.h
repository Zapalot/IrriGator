#pragma once
#include "ArduParWeb.h"
#include "../IndexedFiFo.h"
#include "../Measurements.h"
// generate an SVG graph of the measured data
class BufferedChannelSvgGenerator:public WebFormAbstractUiElement{
    public:
    void generateHtml(String& outputBuffer); ///< Generates a line of a Table
    void reactToRequest(WebServer& server){}; ///< no reactions to parameters
    static void generateSvg(String& outputBuffer,const IndexedFiFo<Measurement>* measurementBuffer,const AbstractMeasurementChannel* channel); //

    IndexedFiFo<Measurement>* measurementBuffer=nullptr; // used to get values
    AbstractMeasurementChannel* channel=nullptr; // used to get descriptions

};
