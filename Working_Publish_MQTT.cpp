#include <Arduino.h>
#include "EspMQTTClient.h"


#define RXp2 16
#define TXp2 17

String Val = "";
String front = "Hydroponics_1/test1/";


EspMQTTClient client(
  "BH-Wireless",
  "Founded2012!",
  "10.1.175.101",  // MQTT Broker server ip
  "iot_admin",   // Can be omitted if not needed
  "controlsadmin",   // Can be omitted if not needed
  "ESP32TestClient",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);

char c;
String dataIn;




void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  Serial1.begin(9600, SERIAL_8N1, RXp2, TXp2);
  
  // Optional functionalities of EspMQTTClient
  //client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
  client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true


  

}



// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  client.subscribe("mytopic/test", [](const String & payload) {
    Serial.println(payload);
  });

  // Subscribe to "mytopic/wildcardtest/#" and display received message to Serial
  client.subscribe("mytopic/wildcardtest/#", [](const String & topic, const String & payload) {
    Serial.println("(From wildcard) topic: " + topic + ", payload: " + payload);
  });

  // Publish a message to "mytopic/test"
  client.publish("mytopic/test", "This is a message"); // You can activate the retain flag by setting the third parameter to true

  // Execute delayed instructions
  client.executeDelayed(5 * 1000, []() {
    client.publish("mytopic/wildcardtest/test123", "This is a message sent 5 seconds later");
  });
}



void loop() {


  client.loop();

  // Subscribe to "mytopic/test" and display received message to Serial
  client.subscribe("Hydroponics_1/test1/output/Light/command", [](const String& payload) {Serial.println("LightCMD: " + payload);Serial1.println("LightCMD: " + payload);});
  


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

    client.publish(topic, Val); // You can activate the retain flag by setting the third parameter to true
    
    c=0;
    dataIn="";
  }



}

