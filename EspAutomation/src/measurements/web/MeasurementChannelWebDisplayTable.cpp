#include "MeasurementChannelWebDisplayTable.h"
// Generates a line of a Table
void MeasurementChannelWebDisplayTable::generateHtml(String &outputBuffer)
{
    outputBuffer += "<table><tr><th>Name</th><th>Description</th><th>Value</th><th>Unit</th></tr>";
    for (auto &channel : channels)
    {
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
    }
    outputBuffer += "</table>";
}