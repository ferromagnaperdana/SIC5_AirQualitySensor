/*Header File*/
#include "Arduino.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
//#include <Ethernet.h>
//#include <EthernetUdp.h>
#include "OTAUpdate.h"
//#include "WebConfig.h"
#include "MQTT_Config.h"
/*------------------------------------*/

/*WiFi Connection Config*/
#ifndef STASSID
#define STASSID "Ferro"
#define STAPSK  "12345678"
#endif

const char* host = "esp32-cyberfreak-webupdate";
const char* ssid = "Ferro";
const char* password = "12345678";
const char* mqtt_server = "ee.unsoed.ac.id";
/*------------------------------------------------------*/
//EthernetClient ethClient;
//PubSubClient client(server, 1883, callback, ethClient, sram);


void setup(void){
  /*ESP32 Serial and I/O Config*/
  Serial.begin(115200);
  delay(100);
  pinMode(DHTPin, INPUT);
  dht.begin();

  pinMode(relay, OUTPUT);
  //digitalWrite(relay, 0);
  /*----------------------------------*/

  /*WiFi Inisialization*/
  WiFi.mode(WIFI_STA);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  /*----------------------------------*/

  /*WebServer */
  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/led_hidup", [](){
    server.send(200, "text/plain", "LED Hidup");
    Serial.println();
    Serial.println("LED Hidup");
    digitalWrite(relay, HIGH);
  });

  server.on("/led_mati", [](){
    server.send(200, "text/plain", "LED Padam");
    Serial.println();
    Serial.println("LED Padam");
    digitalWrite(relay, LOW);
  });

  server.onNotFound(handleNotFound);
  /*------------------------------------------------*/
  
  /*OTA ESP32 Inisialization*/
  OTA_Update();

  /*mDNS Inisialization*/
  if (MDNS.begin("esp32-cyberfreak")) {
    Serial.println("MDNS responder started");
  }
  server.begin();
  Serial.println("HTTP server started");

  for(int i=0; i<17; i=i+8) {
	  chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
	};

  /*HTTP Updater Inisialization*/
  httpUpdater.setup(&server);

  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);

  /*MQTT Inisialization*/
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void loop(void){
  server.handleClient();
  
  ArduinoOTA.handle();
  
  value = digitalRead(relay);

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    //++value;
    //snprintf (msg, MSG_BUFFER_SIZE, "Kondisi LED #%ld", value);
    //Serial.print("Publish message: ");
    //Serial.println(msg);
    //client.publish("outTopic", msg);

    //client.publish("iot22231/H1A020040/LED", String(value).c_str());

    readHumidity = dht.readHumidity();
    readTemperature = dht.readTemperature();

    
    String IP = String(WiFi.localIP().toString()).c_str();
    String ID = String(chipId).c_str();

    Serial.print("ChipID :");
    Serial.println(ID);
    client.publish("iot22231uas/H1A020040/chip_id", String(ID).c_str(), true);

    Serial.print("Alamat IP :");
    Serial.println(IP);
    client.publish("iot22231uas/H1A020040/ip_address", String(IP).c_str(), true);

    Serial.print("Kelembapan :");
    Serial.print(readHumidity);
    Serial.println(" %");
    client.publish("iot22231uas/H1A020040/kelembaban", String(readHumidity).c_str());

    Serial.print("Suhu :");
    Serial.print(readTemperature);
    Serial.println(" *C");
    client.publish("iot22231uas/H1A020040/suhuC", String(readTemperature).c_str());

    Serial.print("Relay :");
    Serial.println(digitalRead(relay));
    client.publish("iot22231uas/H1A020040/relay", String(relay).c_str());


    Serial.println("");
  }
}
