#include "BufferedChannelSvgGenerator.h"

// Generates a line of a Table
void BufferedChannelSvgGenerator::generateHtml(String &outputBuffer)
{
    if(measurementBuffer&&channel)
    generateSvg(outputBuffer,measurementBuffer,channel);
}

void BufferedChannelSvgGenerator::generateSvg(String &outputBuffer, const IndexedFiFo<Measurement> *measurementBuffer, const AbstractMeasurementChannel *channel)
{
    float width = 1000;
    float height = 300;

    float marginFraction = 0.1f;

    float xStart = width * marginFraction;
    float yStart = height * marginFraction;
    float xEnd = width - xStart;
    float yEnd = height - yStart;
    float xScale = (xEnd - xStart) / (float)measurementBuffer->getNumEntriesInBuffer();
    float yScale = (yEnd - yStart) / (channel->getMax() - channel->getMin());
    outputBuffer += R"_(
<svg viewBox="0 0 )_";
    outputBuffer += width;
    outputBuffer += " ";
    outputBuffer += height;
     outputBuffer += R"_(" class="chart">
    <polyline fill="none" stroke="#0074d9" stroke-width="3" points=")_";
    // maybe a smart move to reserve a buffer...
    int bytesPerPoint = 10;
    int bytesFixed = 100;
    outputBuffer.reserve(outputBuffer.length() + measurementBuffer->getNumEntriesInBuffer() * bytesPerPoint + bytesFixed);
    for (int i = 0; i < measurementBuffer->getNumEntriesInBuffer(); i++)
    {
        outputBuffer += (xStart + (float)i * xScale);
        outputBuffer += ",";
        outputBuffer += (int)(yStart + measurementBuffer->getEntry(i).value * yScale);
        outputBuffer += "\n";
    }
    outputBuffer += R"_("/>
    </svg>)_";
}