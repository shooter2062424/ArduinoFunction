/*
   ===========================================
      Copyright (c) 2020 Collin Yang
      https://github.com/shooter2062424
   ===========================================
 */


#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "SimpleList.h"


class Connector{
public:
    Connector(char* ssid, char* pwd, char* username, char* key);
    void Connect();
    void Subscribe(char* name, SubscribeCallbackUInt32Type callb);
    void Subscribe(char* name, SubscribeCallbackDoubleType callb);
    void Subscribe(char* name, SubscribeCallbackBufferType callb);
    void RecieveData(int16_t timeout);
    void UnSubscribe(char* name);
    bool Publish(char* name, char* s);
    bool Publish(char* name, double f);
    bool Publish(char* name, int32_t i);
    bool Publish(char* name, uint32_t i);
    bool Publish(char* name, uint8_t* b, uint16_t len);

private:
    //wifi
    char* SSID;
    char* PWD;

    //MQTT
    static const char* SERVER;      
    static const uint16_t PORT;
    char* USERNAME;
    char* KEY;      

    //handle
    WiFiClient* client;
    Adafruit_MQTT_Client* mqtt;


};

const char* Connector::SERVER = "io.adafruit.com";
const uint16_t Connector::PORT = 1883;

Connector::Connector(char* ssid, char* pwd, char* username, char* key){
  this->SSID = ssid;
  this->PWD = pwd;
  this->USERNAME = username;
  this->KEY = key;
  client = new WiFiClient();
  mqtt = new Adafruit_MQTT_Client(client, SERVER, PORT, USERNAME, KEY);  

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
}

void Connector::Connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt->connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt->connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt->connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt->disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}

void  Connector::Subscribe(char* name, SubscribeCallbackUInt32Type callb){
    Adafruit_MQTT_Subscribe* sub = new Adafruit_MQTT_Subscribe(mqtt, name, MQTT_QOS_1);
    sub->setCallback(callb);
    mqtt->subscribe(sub);
}

void  Connector::Subscribe(char* name, SubscribeCallbackDoubleType callb){
    Adafruit_MQTT_Subscribe* sub = new Adafruit_MQTT_Subscribe(mqtt, name, MQTT_QOS_1);
    sub->setCallback(callb);
    mqtt->subscribe(sub);  
}

void  Connector::Subscribe(char* name, SubscribeCallbackBufferType callb){
    Adafruit_MQTT_Subscribe* sub = new Adafruit_MQTT_Subscribe(mqtt, name, MQTT_QOS_1);
    sub->setCallback(callb);
    mqtt->subscribe(sub);  
}

void  Connector::RecieveData(int16_t timeout = 10000){
    mqtt->processPackets(timeout);
}

void Connector::UnSubscribe(char* name){
  
}

bool Connector::Publish(char* name, char* s){
    Adafruit_MQTT_Publish pub = Adafruit_MQTT_Publish(mqtt, name);  
    return pub.publish(s);
}

bool Connector::Publish(char* name, double f){
    Adafruit_MQTT_Publish pub = Adafruit_MQTT_Publish(mqtt, name);  
    return pub.publish(f);  
}

bool Connector::Publish(char* name, int32_t i){
    Adafruit_MQTT_Publish pub = Adafruit_MQTT_Publish(mqtt, name);  
    return pub.publish(i);  
}

bool Connector::Publish(char* name, uint32_t i){
    Adafruit_MQTT_Publish pub = Adafruit_MQTT_Publish(mqtt, name);  
    return pub.publish(i);  
}

bool Connector::Publish(char* name, uint8_t* b, uint16_t len){
    Adafruit_MQTT_Publish pub = Adafruit_MQTT_Publish(mqtt, name);  
    return pub.publish(b, len);  
}
