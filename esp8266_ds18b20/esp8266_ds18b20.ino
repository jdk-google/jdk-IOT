#include <time.h>
#include <ESP.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
//run Sketch => IncludeLibraries => ManageLibraries - install both


// Replace these with your WiFi network settings
const char* ssid = "TP-LINK_31AD"; //replace this with your WiFi network name
const char* password = "41064726"; //replace this with your WiFi network password

void ConnectWifi() {
  Serial.println("Connecting to network: "+String(ssid));
   
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected!");
}

void SyncTime() {
  configTime(1 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (time(nullptr) < 1500000000) {
    Serial.print(".");
    delay(200);
  }
  Serial.println("Time synced!");
}
    
void SendTemperature (float Temperature) {
  Serial.print("Sending Temperature: ");
  Serial.println(Temperature);
  
  String host="http://testowy-projeklt.appspot.com";
  time_t now = time(nullptr);
  Serial.println(now);
  String url = "/saveTemperature?temp=" + 
               String(Temperature) + 
               "&device_id=ESP8266-" + String(ESP.getChipId())+
               "&timestamp="+ String(int(now))+
               "&ssid="+ssid+
               "&hostname=esp8266-"+ String(ESP.getChipId());

  Serial.println("Making HTTPConnection to: " + host + url);
  HTTPClient http;
  http.begin(host+url);  
  int httpCode = http.GET();   
  Serial.println(httpCode);
  String payload = http.getString();   //Get the request response payload
  Serial.print("Response:");

  Serial.println(payload);                     //Print the response payload
}

#define ONE_WIRE_BUS D4  // DS18B20 pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);


void setup() {
  delay(100);
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  Serial.println();
  Serial.println("Starting");
  
  ConnectWifi();
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);
  //
  SyncTime();
  //
  // setup OneWire bus
  DS18B20.begin();
}

void loop() {
  digitalWrite(BUILTIN_LED, LOW);   // turn the LED on (HIGH is the voltage level)
  //
  DS18B20.requestTemperatures(); 
  float temp;
  temp = DS18B20.getTempCByIndex(0);
  SendTemperature(temp);
  delay(200);

  digitalWrite(BUILTIN_LED, HIGH);    // turn the LED off by making the voltage LOW
  delay(90000);              // wait for a second
  
}
