#include "ArduPar3.h"
#include "ArduParWeb.h"

ArduPar3Collection parameterCollection; // create a collection for parameters so we can conveniently do things with all of them at once
CallbackArduPar3 dumpCallbackParameter;
CallbackArduPar3 loadCallbackParameter;
CallbackArduPar3 saveCallbackParameter;

//////////////////////////Everything WiFi
#include <esp_wifi.h>
#include <WiFi.h>
#include "credentials.h"
void setupWifi(const char *ssid, const char *password);

///////////////////everything Web///
#include <WiFiClient.h>
#include <WebServer.h>

WebServer server(80);
void notFound();           // function to call on unknon URIs
void serveArduParFormUi(); // function to serve our UI

#include "WebFormArduParUiEntry.h"
#include "WebFormSection.h"
#include "WebFormHtmlPage.h"
#include "WebFormDebugElement.h"
#include "WebFormArduParUiBuilder.h"

WebFormArduParUiBuilder arduParUi;
WebFormDebugElement debugSection(&server);

//////////////Here comes the actual automation

////Time related things
#define NTP_SERVER "pool.ntp.org"
#define UTC_OFFSET 0
#define UTC_OFFSET_DST 0

#include <time.h>
#include "TimeBase.h"
SystemTimeBase timeBase; // this is what we use while the code is running

#include <Wire.h>
#include <RTClib.h> // this one we use to set the time once on startup
#define rtcClockPin 21
#define rtcDataPin 22

// read external RTC to set internal clock
void setSytemClockFromRtc()
{
    TwoWire rtcWire(0);
    RTC_DS3231 rtc;
    rtcWire.begin(rtcDataPin, rtcClockPin, 10000);
    if (!rtc.begin(&rtcWire))
    {
        Serial.println(F("Couldn't find RTC!"));
        return;
    }
    if (rtc.lostPower())
    {
        Serial.println(F("RTC lost power,you need to set the time!"));
        rtc.adjust(DateTime(1718298024));
        return;
    }
    DateTime now = rtc.now();
    char datebuffer[] = "DDD, DD MMM YYYY hh:mm:ss";
    now.toString(datebuffer); // this crashes unfortunately
    Serial.print("Time read from RTC is: ");
    Serial.println(datebuffer);
    timeval curTime;
    curTime.tv_sec = now.unixtime(); // not year 2038 save!
    curTime.tv_usec = 0;
    settimeofday(&curTime, NULL);
}

#include "LedcPwmPin.h"
#define nValves 4
int valvePinNumbers[] = {26, 27, 32, 33};
LedcPwmPin valvePins[nValves];

#include "PulseSustainActuator.h"
const char *valveActuatorNames[] = {"/actuators/valve1", "/actuators/valve2", "/actuators/valve3", "/actuators/valve4"};
PulseSustainActuator valveActuators[nValves];

#include "ScheduledOnOffController.h"
const char *valveControllerNames[] = {"/schedules/valve1", "/schedules/valve2", "/schedules/valve3", "/schedules/valve4"};
ScheduledOnOffController valveControllers[nValves];

//////////Measurements and logging
int sdCsPin = 25;
#include <vector>
#include "src/measurements/Measurements.h"
#include "src/measurements/BufferedChannelCollection.h"
#include "src/measurements/sources/DummyMeasurementChannel.h"
#include "src/measurements/logging/LogTargets.h"
#include "src/measurements/logging/SdCardLogTarget.h"
#include "src/measurements/logging/BufferedMeasurementLogger.h"

#include "src/measurements/web/MeasurementChannelWebDisplayTable.h"
#include "src/measurements/web/ChannelCollectionWebView.h"
DummyMeasurementChannel counterChannel;
std::vector<AbstractMeasurementChannel *> measurementChannels; // here we keep all out measurements
BufferedChannelCollection measurementBuffer;                   // we use this to keep a number of past measurements in memory (i.e. for displaying them in a graph)
SdCardLogTarget logTargetSd;
SerialLogTarget logTargetSerial;
BufferedMeasurementLogger measurementLogger;
WebFormHtmlPage sensorPage("Sensor States");
MeasurementChannelWebDisplayTable sensorTable;
ChannelCollectionWebView sensorWebDisplay(&measurementBuffer);
void downloadLogFile(); // instruct the server to download the logfile

void setup()
{
    Serial.begin(115200);
    setSytemClockFromRtc();
    // configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
    // set up parameter infrastructure
    ArduPar3Collection::globalDefaultCollection = &parameterCollection; // use our parameter collection as a default
    dumpCallbackParameter.setup(PSTR("/commands/dump"), PSTR("Print Parameter Summary"), []
                                { parameterCollection.dumpParameterInfos(&Serial); });
    loadCallbackParameter.setup(PSTR("/commands/load"), PSTR("Load Settings."), []
                                { parameterCollection.loadAll(); });
    saveCallbackParameter.setup(PSTR("/commands/save"), PSTR("Save Settings."), []
                                { parameterCollection.saveAll(); });

    // set up automation
    for (int i = 0; i < nValves; i++)
    {
        valvePins[i].setup(valvePinNumbers[i], 20000, 8);
        valveActuators[i].setup(valveActuatorNames[i], "Valve actuator", &valvePins[i], 500, 255, 128, 0);
        valveControllers[i].setup(&valveActuators[i], valveControllerNames[i], "On/Off Schedule", &timeBase, ScheduledOnOffController::ScheduleTimeBaseHours, 0, 24, 1);
    }
    /// try to get connected to a WiFi
    setupWifi(WIFI_SSID, WIFI_PASSWORD);
    /// setup server
    server.on("/", serveArduParFormUi);
    server.onNotFound(notFound);
    server.begin();

    // build user interface
    arduParUi.buildUi(ArduPar3Collection::globalDefaultCollection, "/");

    parameterCollection.loadAll();
    //parameterCollection.dumpParameterInfos(&Serial);

    // set up measurements and logging
    measurementChannels.reserve(1);
    measurementChannels.push_back(&counterChannel);
    measurementBuffer.setup(&measurementChannels, 128);
    measurementBuffer.minPushIntervalMillis = 10;
    logTargetSd.setup("/test.txt", sdCsPin);
    measurementLogger.setup(&measurementBuffer, &logTargetSd);

    // set up page for displaying sensor values
    sensorTable.channels = measurementChannels;
    sensorPage.uiElements.push_back(&sensorWebDisplay);
    server.on("/sense", []()
              { sensorPage.serveYourself(server); });
             
    // allow logfile download
    server.on("/log", downloadLogFile);
}

void loop()
{
    // run measurements and logging
    counterChannel.update();
    measurementBuffer.pushMeasurementsToBufferInterval();
    measurementLogger.update();
    // update parameters from web and serial

    parameterCollection.updateParametersFromStream(&Serial, 1000);
    server.handleClient();
    /*
        // update controllers + actuators
        Serial.print("Hours of year:\t");
        Serial.print(timeBase.getHoursOfTheDay());
        Serial.print("\tValve States:\t");
        for (int i = 0; i < nValves; i++)
        {
            Serial.print(valvePins[i].getOutput());
            Serial.print("\t");
            valveControllers[i].update();
        }
        Serial.print("\n");
        */
    delay(2); // allow the cpu to switch to other tasks
}

void setupWifi(const char *ssid, const char *password)
{

    Serial.print("Connecting to wifi network \"");
    Serial.print(ssid);
    Serial.print("\" .");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    long start = millis();
    // try for ten seconds to connect every 500 ms (i.e. make 10000/500 = 20 attempts)
    while (WiFi.status() != WL_CONNECTED && millis() - start < 4000)
    {
        Serial.print(".");
        delay(500);
    }

    // print result of connection attempt(s) on serial console
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(" failed with status ");
        Serial.println(WiFi.status());
    }
    else
    {
        Serial.println(" succeeded");
        Serial.print("local IP address is ");
        Serial.println(WiFi.localIP());
        esp_wifi_set_ps(WIFI_PS_NONE);
    }
}
// function to serve our UI
void serveArduParFormUi()
{

    String message = "_________new Request!\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    Serial.println(message);

    String pageContent;
    pageContent.reserve(8192);
    arduParUi.reactToRequest(server);
    arduParUi.generateHtml(pageContent);
    server.send(200, "text/html", pageContent);
}

void notFound()
{
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}
// instruct the server to download the logfile
void downloadLogFile()
{
    File download = SD.open(logTargetSd.getFilename(), "r");
    if (download)
    {
        server.sendHeader("Content-Type", "text/text");
        server.sendHeader("Content-Disposition", String("attachment; filename=") + logTargetSd.getFilename());
        server.sendHeader("Connection", "close");
        server.streamFile(download, "application/octet-stream");
        download.close();
    }
    else
    {
        String message = "Couldn't open file for download.\n";
        server.send(404, "text/plain", message);
    }
}