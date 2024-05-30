#include <WString.h>

String SendHTML(float Temperature,float Humidity, String IP, float chipId){
 String ptr = "<!DOCTYPE html> <html>\n";

 ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
 ptr +="<title>Proyek dengan ESP32</title>\n";
 ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
 ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
 ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
 ptr +="</style>\n";
 ptr +="</head>\n";
 ptr +="<body>\n";
 ptr +="<div id=\"identitas\">\n";
 ptr +="<h1>Ferro Magna Perdana</h1>\n";
 ptr +="<p>NIM: H1A020040 </p>";
 ptr +="<p>Jurusan Teknik Elektro FT Unsoed </p>";
 ptr +="</div>\n";
 ptr +="<div id=\"identitas_perangkat\">\n";
 ptr +="<h1>Identitas Perangkat</h1>\n";
 ptr +="<p>IP Address</p>";
 ptr +=IP;
 //ptr +="<p>Chip Model</p>";
 //ptr +=chipModel;
 ptr +="<p>Chip ID</p>";
 ptr +=chipId;
 ptr +="</div>\n";
 ptr +="<div id=\"webpage\">\n";
 ptr +="<h1>Pembacaan suhu dan kelembaban</h1>\n";
 ptr +="<p>Suhu: ";
 ptr +=Temperature;
 ptr +=" <sup>o</sup>C</p>";
 ptr +="<p>Kelembaban: ";
 ptr +=Humidity;
 ptr +=" %</p>";
 ptr +="</div>\n";
 ptr +="<div id=\"saklar\">\n";
 ptr +="<h1>Kendali LED</h1>\n";
 ptr +="<a href=\"/led_hidup\"><button>ON</button></a>";
 ptr +="<a href=\"/led_mati\"><button>OFF</button></a>";
 ptr +="<h1>Update Firmware</h1>\n";
 ptr +="<a href=\"/update\"><button>Update</button></a>";
 ptr +="</div>\n";
 ptr +="</body>\n";
 ptr +="</html>\n";
 return ptr; 
}
