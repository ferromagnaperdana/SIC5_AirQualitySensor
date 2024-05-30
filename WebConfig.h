#include <WebServer.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <HTTPUpdateServer.h>
#include <AutoConnect.h>
#include "bacaDHT.h"
#include "SendHTML2.h"

WebServer server(80);
HTTPUpdateServer httpUpdater;

uint32_t chipId = 0;
uint32_t chipModel = 0;
const int relay = 26;

void handleRoot() {
  String message = "Selamat datang di ESP32!\n\n";
  message += "Nama : Ferro Magna Perdana \nNIM : H1A020040 \nUniversitas Jenderal Soedirman\n";
  message += "Chip Model = ";
  message += String(ESP.getChipModel()).c_str();
  message += "\nChipID = ";
  message += String(chipId).c_str();
  message += "\nMyIP Address = ";
  message += String(WiFi.localIP().toString().c_str());
  //server.send(200, "text/plain", message);
  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity
  String IP = WiFi.localIP().toString().c_str();
  //chipModel = String(ESP.getChipModel()).c_str(); 
  server.send(200, "text/html", SendHTML(Temperature, Humidity, IP, chipId));
}

void handleNotFound(){
  digitalWrite(relay, 1);
  String message = "File Not Found\n\n";
  message += "URL: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(relay, 0);
}