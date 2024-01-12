
/**************************************************************************************************************************************************
* File name     : 
* Compiler      : 
* Autor         : 
* Created       : 
* Modified      : 
* Last modified :
*
*
* Description   : 
*
* Other info    : 
**************************************************************************************************************************************************/


/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------Include Files----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/

/**************************************************************************************************************************************************
* File name     : 
* Compiler      : 
* Autor         : 
* Created       : 
* Modified      : 
* Last modified :
*
*
* Description   : 
*
* Other info    : 
**************************************************************************************************************************************************/


/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------Include Files----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
#include <Arduino.h>
#include "EspMQTTClient.h"
#include <ArduinoJson.h>
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------Local definitions------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
#include <ArduinoJson.h>
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------Local definitions------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------I/O Definitions--------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------I/O Definitions--------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
#define RXp2 16
#define TXp2 17

String Val = "";
String MqttPath = "Hydroponics_1/test1/";

String MqttPath = "Hydroponics_1/test1/";

/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------ Configuration --------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/

EspMQTTClient client(
  "BH-Wireless",
  "Founded2012!",
  "10.1.175.101",  // MQTT Broker server ip
  "iot_admin",   // Can be omitted if not needed
  "controlsadmin",   // Can be omitted if not needed
  "ESP32TestClient",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);

//Variables used for MQTT Discory on Home Assistant
const char* g_deviceModel = "ESP32Device";
const char* g_swVersion = "1.1";
const char* g_manufacturer = "HiLetGo";
String g_deviceName = "Hydroponics";
String g_mqttStatusTopic = "esp32iotsenosr/" + g_deviceName;

/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------Public variables-------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/


//Variables used for MQTT Discory on Home Assistant
const char* g_deviceModel = "ESP32Device";
const char* g_swVersion = "1.1";
const char* g_manufacturer = "HiLetGo";
String g_deviceName = "Hydroponics";
String g_mqttStatusTopic = "esp32iotsenosr/" + g_deviceName;

/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------Public variables-------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/


char c;
String dataIn;
unsigned long       g_Time = 0;
int                 g_count = 0; 
int                 g_mqttCounterConn = 0;
bool                g_InitSystem = true;
String              g_UniqueId;
bool                g_SendMqttData = false;

/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------ prototypes for references---------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/

void MqttHomeAssistantDiscovery();

/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------ SETUP ----------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
unsigned long       g_Time = 0;
int                 g_count = 0; 
int                 g_mqttCounterConn = 0;
bool                g_InitSystem = true;
String              g_UniqueId;
bool                g_SendMqttData = false;

/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------ prototypes for references---------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/

void MqttHomeAssistantDiscovery();

/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------ SETUP ----------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  Serial1.begin(9600, SERIAL_8N1, RXp2, TXp2);
  
  Serial.println("");
  Serial.println("");
  Serial.println("----------------------------------------------");
  Serial.print("MODEL: ");
  Serial.println(g_deviceModel);
  Serial.print("DEVICE: ");
  Serial.println(g_deviceName);
  Serial.print("SW Rev: ");
  Serial.println(g_swVersion);
  Serial.println("----------------------------------------------");

  
  Serial.println("");
  Serial.println("");
  Serial.println("----------------------------------------------");
  Serial.print("MODEL: ");
  Serial.println(g_deviceModel);
  Serial.print("DEVICE: ");
  Serial.println(g_deviceName);
  Serial.print("SW Rev: ");
  Serial.println(g_swVersion);
  Serial.println("----------------------------------------------");

  // Optional functionalities of EspMQTTClient
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
  client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  client.enableLastWillMessage("TestClient/lastwill", "offline");  // You can activate the retain flag by setting the third parameter to true

  byte mac[6];
  WiFi.macAddress(mac);
  g_UniqueId =  String(mac[0],HEX) +String(mac[1],HEX) +String(mac[2],HEX) +String(mac[3],HEX) + String(mac[4],HEX) + String(mac[5],HEX);
  Serial.println("INIT SYSTEM...");
  Serial.println(g_UniqueId);
  byte mac[6];
  WiFi.macAddress(mac);
  g_UniqueId =  String(mac[0],HEX) +String(mac[1],HEX) +String(mac[2],HEX) +String(mac[3],HEX) + String(mac[4],HEX) + String(mac[5],HEX);
  Serial.println("INIT SYSTEM...");
  Serial.println(g_UniqueId);

  

}



// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{


  
  //MqttHomeAssistantDiscovery();     // Send Discovery Data

  
  //MqttHomeAssistantDiscovery();     // Send Discovery Data

  // Subscribe to "mytopic/test" and display received message to Serial
  client.subscribe(MqttPath + "output/Light/command", [](const String& payload) {Serial.println("here");Serial.println("LightCMD: " + payload);Serial1.println("LightCMD: " + payload);});
  client.subscribe(MqttPath + "output/Water Pump/command", [](const String& payload) {Serial.println("Water Pump CMD: " + payload);Serial1.println("Water Pump CMD: " + payload);});
  client.subscribe(MqttPath + "output/pump1/command", [](const String& payload) {Serial.println("pump1CMD: " + payload);Serial1.println("pump1CMD: " + payload);});
  client.subscribe(MqttPath + "output/pump2/command", [](const String& payload) {Serial.println("pump2CMD: " + payload);Serial1.println("pump2CMD: " + payload);});
  client.subscribe(MqttPath + "output/pump3/command", [](const String& payload) {Serial.println("pump3CMD: " + payload);Serial1.println("pump3CMD: " + payload);});
  client.subscribe(MqttPath + "output/pump4/command", [](const String& payload) {Serial.println("pump4CMD: " + payload);Serial1.println("pump4CMD: " + payload);});
  client.subscribe(MqttPath + "state/Mode", [](const String& payload) {Serial.println("Mode: " + payload);Serial1.println("Mode: " + payload);});  
  client.publish("TestClient/lastwill", "online");
}



void loop() {


  client.loop();

  while(Serial1.available() ) //Receive data from Uno
  {
    c = Serial1.read(); //Read the incoming data and save to c variable, once character at a time

    if(c != '\n') { dataIn += c; } //If the End Data Identifier ('\n') not found, then append data c to the string variable dataIn
    else          { break;}        //If the end Data identifier ('\n') is found, break while loop
  }

  if(c=='\n')
  {

    //Serial.println(dataIn);

    int index = dataIn.indexOf(":");
    String Name = dataIn.substring(0, index);
    Val = dataIn.substring(index);
    Val.replace(":","");
    Val.trim();
    
    String topic = MqttPath + Name;
    String topic = MqttPath + Name;

    client.publish(topic, Val); // You can activate the retain flag by setting the third parameter to true
    
    c=0;
    dataIn="";
  }



}

void MqttHomeAssistantDiscovery()
{
    String discoveryTopic;
    String payload;
    String strPayload;
    if(true)
    {
        Serial.println("SEND HOME ASSISTANT DISCOVERY!!!");
        JsonDocument payload;
        char buffer[256];
        JsonObject device;
        JsonArray identifiers;


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //PH
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        discoveryTopic = "homeassistant/sensor/" + g_deviceName + "_PH" + "/config";
        
        //payload["name"] = g_deviceName + ".PH";
        payload["device_class"] = "pH";
        payload["state_topic"] = g_mqttStatusTopic;
        payload["unit_of_measurement"] = "pH";
        payload["value_template"] = "{{ value_json.pH}}";
        payload["unique_id"] = g_UniqueId + "_PH";
        
        //device = payload.createNestedObject("device");
        //device["name"] = g_deviceName;
        //device["model"] = g_deviceModel;
        // device["sw_version"] = g_swVersion;
        // device["manufacturer"] = g_manufacturer;
        //identifiers = device.createNestedArray("identifiers");
        // identifiers.add(g_UniqueId);
        size_t n = serializeJson(payload, buffer);
        serializeJsonPretty(payload, Serial);
        Serial.println(" ");

        client.publish(discoveryTopic.c_str(), buffer, n);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Humidity
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // payload.clear();
        // device.clear();
        // identifiers.clear();
        // strPayload.clear();

        // discoveryTopic = "homeassistant/sensor/esp32iotsensor/" + g_deviceName + "_hum" + "/config";
        
        // payload["name"] = g_deviceName + ".hum";
        // payload["uniq_id"] = g_UniqueId + "_hum";
        // payload["stat_t"] = g_mqttStatusTopic;
        // payload["dev_cla"] = "humidity";
        // payload["val_tpl"] = "{{ value_json.hum | is_defined }}";
        // payload["unit_of_meas"] = "%";
        // device = payload.createNestedObject("device");
        // device["name"] = g_deviceName;
        // device["model"] = g_deviceModel;
        // device["sw_version"] = g_swVersion;
        // device["manufacturer"] = g_manufacturer;
        // identifiers = device.createNestedArray("identifiers");
        // identifiers.add(g_UniqueId);

        // serializeJsonPretty(payload, Serial);
        // Serial.println(" ");
        // serializeJson(payload, strPayload);

        // g_mqttPubSub.publish(discoveryTopic.c_str(), strPayload.c_str());

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Binary Door
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // payload.clear();
        // device.clear();
        // identifiers.clear();
        // strPayload.clear();

        // discoveryTopic = "homeassistant/binary_sensor/esp32iotsensor/" + g_deviceName + "_door" + "/config";
        
        // payload["name"] = g_deviceName + ".door";
        // payload["uniq_id"] = g_UniqueId + "_door";
        // payload["stat_t"] = g_mqttStatusTopic;
        // payload["dev_cla"] = "door";
        // payload["val_tpl"] = "{{ value_json.inputstatus | is_defined }}";
        // device = payload.createNestedObject("device");
        // device["name"] = g_deviceName;
        // device["model"] = g_deviceModel;
        // device["sw_version"] = g_swVersion;
        // device["manufacturer"] = g_manufacturer;
        // identifiers = device.createNestedArray("identifiers");
        // identifiers.add(g_UniqueId);

        // serializeJsonPretty(payload, Serial);
        // Serial.println(" ");
        // serializeJson(payload, strPayload);

        // g_mqttPubSub.publish(discoveryTopic.c_str(), strPayload.c_str());

    }
}void MqttHomeAssistantDiscovery()
{
    String discoveryTopic;
    String payload;
    String strPayload;
    if(true)
    {
        Serial.println("SEND HOME ASSISTANT DISCOVERY!!!");
        JsonDocument payload;
        char buffer[256];
        JsonObject device;
        JsonArray identifiers;


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //PH
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        discoveryTopic = "homeassistant/sensor/" + g_deviceName + "_PH" + "/config";
        
        //payload["name"] = g_deviceName + ".PH";
        payload["device_class"] = "pH";
        payload["state_topic"] = g_mqttStatusTopic;
        payload["unit_of_measurement"] = "pH";
        payload["value_template"] = "{{ value_json.pH}}";
        payload["unique_id"] = g_UniqueId + "_PH";
        
        //device = payload.createNestedObject("device");
        //device["name"] = g_deviceName;
        //device["model"] = g_deviceModel;
        // device["sw_version"] = g_swVersion;
        // device["manufacturer"] = g_manufacturer;
        //identifiers = device.createNestedArray("identifiers");
        // identifiers.add(g_UniqueId);
        size_t n = serializeJson(payload, buffer);
        serializeJsonPretty(payload, Serial);
        Serial.println(" ");

        client.publish(discoveryTopic.c_str(), buffer, n);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Humidity
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // payload.clear();
        // device.clear();
        // identifiers.clear();
        // strPayload.clear();

        // discoveryTopic = "homeassistant/sensor/esp32iotsensor/" + g_deviceName + "_hum" + "/config";
        
        // payload["name"] = g_deviceName + ".hum";
        // payload["uniq_id"] = g_UniqueId + "_hum";
        // payload["stat_t"] = g_mqttStatusTopic;
        // payload["dev_cla"] = "humidity";
        // payload["val_tpl"] = "{{ value_json.hum | is_defined }}";
        // payload["unit_of_meas"] = "%";
        // device = payload.createNestedObject("device");
        // device["name"] = g_deviceName;
        // device["model"] = g_deviceModel;
        // device["sw_version"] = g_swVersion;
        // device["manufacturer"] = g_manufacturer;
        // identifiers = device.createNestedArray("identifiers");
        // identifiers.add(g_UniqueId);

        // serializeJsonPretty(payload, Serial);
        // Serial.println(" ");
        // serializeJson(payload, strPayload);

        // g_mqttPubSub.publish(discoveryTopic.c_str(), strPayload.c_str());

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Binary Door
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // payload.clear();
        // device.clear();
        // identifiers.clear();
        // strPayload.clear();

        // discoveryTopic = "homeassistant/binary_sensor/esp32iotsensor/" + g_deviceName + "_door" + "/config";
        
        // payload["name"] = g_deviceName + ".door";
        // payload["uniq_id"] = g_UniqueId + "_door";
        // payload["stat_t"] = g_mqttStatusTopic;
        // payload["dev_cla"] = "door";
        // payload["val_tpl"] = "{{ value_json.inputstatus | is_defined }}";
        // device = payload.createNestedObject("device");
        // device["name"] = g_deviceName;
        // device["model"] = g_deviceModel;
        // device["sw_version"] = g_swVersion;
        // device["manufacturer"] = g_manufacturer;
        // identifiers = device.createNestedArray("identifiers");
        // identifiers.add(g_UniqueId);

        // serializeJsonPretty(payload, Serial);
        // Serial.println(" ");
        // serializeJson(payload, strPayload);

        // g_mqttPubSub.publish(discoveryTopic.c_str(), strPayload.c_str());

    }
}