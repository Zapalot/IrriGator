#include "ArduParWeb.h"
#include "../Measurements.h"
#include "../BufferedChannelCollection.h"
#include "BufferedChannelSvgGenerator.h"
class ChannelCollectionWebView: public WebFormAbstractUiElement{
    public:
    ChannelCollectionWebView(BufferedChannelCollection* collection);
    void generateHtml(String& outputBuffer); ///< Generates a line of a Table
    void reactToRequest(WebServer& server){}; ///< no reactions to parameters
    protected:
    BufferedChannelCollection* collection=nullptr;
};
