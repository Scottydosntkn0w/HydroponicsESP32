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

#include <WiFi.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------Local definitions------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------I/O Definitions--------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
#define RXp2 16
#define TXp2 17

/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------ Configuration --------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/


const char*         g_ssid = "BH-Wireless";
const char*         g_password = "Founded2012!";
const char*         g_mqtt_server = "10.1.175.101";                           // MQTT Server IP, same of Home Assistant
const char*         g_mqttUser = "iot_admin";                                 // MQTT Server User Name
const char*         g_mqttPsw = "controlsadmin";                                   // MQTT Server password
int                 g_mqttPort = 1883;                                        // MQTT Server Port


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
String Val = "";
String front = "Hydroponics_1/test1/";
String discoery_prefix = "homeassistant";
String node_id = "ESP32_HYDRO";

WiFiClient          g_WiFiClient;
PubSubClient        g_mqttPubSub(g_WiFiClient);
unsigned long       g_Time = 0;
int                 g_count = 0; 
int                 g_mqttCounterConn = 0;
bool                g_InitSystem = true;
String              g_UniqueId;
bool                g_SendMqttData = false;

/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------ prototypes for references---------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
void MqttReceiverCallback(char* topic, byte* inFrame, unsigned int length);
void setup_wifi();
void MqttHomeAssistantDiscovery();
void MqttReconnect();

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
  

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Wifi Init
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  setup_wifi();

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // MQTT Init
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  g_mqttPubSub.setServer(g_mqtt_server, g_mqttPort);
  g_mqttPubSub.setCallback(MqttReceiverCallback);

}



void loop() 
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // MQTT Connection
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(WiFi.status() == WL_CONNECTED)
    {
        if(!g_mqttPubSub.connected())
            MqttReconnect();
        else
            g_mqttPubSub.loop();
    }


    if(g_InitSystem)
    {
        delay(100);
        g_InitSystem = false;
        Serial.println("INIT SYSTEM...");
        MqttHomeAssistantDiscovery();     // Send Discovery Data
    }














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
      
      String topic = front + Name;

      //client.publish(topic, Val); // You can activate the retain flag by setting the third parameter to true
      
      c=0;
      dataIn="";
    }



















    //if(millis() - g_Time > PERIOD_MILLSEC_500)  // Every 500 [msec]
    //{

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // SEND MQTT DATA
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
        if(g_SendMqttData == true)        
        {
            StaticJsonDocument<200> payload;  
            //payload["temp"] = g_Temperature;
            //payload["hum"] = g_Humidity;
            //payload["inputstatus"] = g_strDoorStatus;

            String strPayload;
            serializeJson(payload, strPayload);

            if(g_mqttPubSub.connected())
            {
                g_mqttPubSub.publish(g_mqttStatusTopic.c_str(), strPayload.c_str()); 
                Serial.println("MQTT: Send Data!!!");
                Serial.println(" ");
                Serial.println(" ");
                g_SendMqttData = false;
            }
        //}
    }










  //client.loop();

  // Subscribe to "mytopic/test" and display received message to Serial
  //client.subscribe("Hydroponics_1/test1/output/Light/command", [](const String& payload) {Serial.println("LightCMD: " + payload);Serial1.println("LightCMD: " + payload);});

  






}

/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------ Public Functions -----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
void setup_wifi() 
{
    int counter = 0;
    byte mac[6];
    delay(10);
    // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(g_ssid);

    WiFi.begin(g_ssid, g_password);

    WiFi.macAddress(mac);
    g_UniqueId =  String(mac[0],HEX) +String(mac[1],HEX) +String(mac[2],HEX) +String(mac[3],HEX) + String(mac[4],HEX) + String(mac[5],HEX);

    Serial.print("Unique ID: ");
    Serial.println(g_UniqueId);    
   
    while(WiFi.status() != WL_CONNECTED && counter++ < 8) 
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("");

    if(WiFi.status() == WL_CONNECTED)
    {
        Serial.println("WiFi connected");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    } else
    {
        Serial.println("WiFi NOT connected!!!");
    }
}

void MqttReconnect() 
{
    // Loop until we're reconnected
    while (!g_mqttPubSub.connected()  && (g_mqttCounterConn++ < 4))
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (g_mqttPubSub.connect(g_deviceName.c_str(), g_mqttUser, g_mqttPsw)) 
        {
            Serial.println("connected");
            // Subscribe
            g_mqttPubSub.subscribe("homeassistant/status");
            delay(100);
        } else 
        {
            Serial.print("failed, rc=");
            Serial.print(g_mqttPubSub.state());
            Serial.println(" try again in 1 seconds");
            delay(1000);
        }
    }  
    g_mqttCounterConn = 0;
}

void MqttHomeAssistantDiscovery()
{
    String discoveryTopic;
    String payload;
    String strPayload;
    if(g_mqttPubSub.connected())
    {
        Serial.println("SEND HOME ASSISTANT DISCOVERY!!!");
        StaticJsonDocument<600> payload;
        JsonObject device;
        JsonArray identifiers;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //PH
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        discoveryTopic = "homeassistant/sensor/esp32iotsensor/" + g_deviceName + "_PH" + "/config";
        
        payload["name"] = g_deviceName + ".PH";
        payload["uniq_id"] = g_UniqueId + "_PH";
        payload["stat_t"] = g_mqttStatusTopic;
        payload["dev_cla"] = "PH";
        payload["val_tpl"] = "{{ value_json.ph | is_defined }}";
        payload["unit_of_meas"] = "pH";
        device = payload.createNestedObject("device");
        device["name"] = g_deviceName;
        device["model"] = g_deviceModel;
        device["sw_version"] = g_swVersion;
        device["manufacturer"] = g_manufacturer;
        identifiers = device.createNestedArray("identifiers");
        identifiers.add(g_UniqueId);

        serializeJsonPretty(payload, Serial);
        Serial.println(" ");
        serializeJson(payload, strPayload);

        g_mqttPubSub.publish(discoveryTopic.c_str(), strPayload.c_str());

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

void MqttReceiverCallback(char* topic, byte* inFrame, unsigned int length) 
{
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    byte state = 0;
    String messageTemp;
    
    for (int i = 0; i < length; i++) 
    {
        Serial.print((char)inFrame[i]);
        messageTemp += (char)inFrame[i];
    }
    Serial.println();
  
    if(String(topic) == String("homeassistant/status")) 
    {
        if(messageTemp == "online")
            MqttHomeAssistantDiscovery();
    }
}