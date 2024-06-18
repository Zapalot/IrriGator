#include "ChannelCollectionWebView.h"
ChannelCollectionWebView::ChannelCollectionWebView(BufferedChannelCollection *collection):
collection(collection)
{
}

// Generates a line of a Table
void ChannelCollectionWebView::generateHtml(String &outputBuffer)
{

    auto channels = collection->getChannels();
    auto buffers = collection->getChannelBuffers();
    outputBuffer += "<table><tr><th>Name</th><th>Description</th><th>Value</th><th>Unit</th></tr>";
    for (int i = 0; i < channels->size(); i++)
    {
        auto &channel = (*channels)[i];
        auto &buffer = (*buffers)[i];
        outputBuffer += "<tr><td>";
        outputBuffer += channel->getShortName();
        outputBuffer += "</td><td>";
        outputBuffer += channel->getDescription();
        outputBuffer += "</td><td>";
        outputBuffer += channel->getLastMeasurement().value;
        outputBuffer += "</td><td>";
        outputBuffer += channel->getUnit();
        outputBuffer += "</td><td>";
        outputBuffer += "</td></tr>";

        outputBuffer += "<tr><td colspan=\"4\">";
        // insert an SVG graphof the past values
        BufferedChannelSvgGenerator::generateSvg(outputBuffer, &buffer, channel);
        outputBuffer += "</td></tr>";
    }
    outputBuffer += "</table>";
}
