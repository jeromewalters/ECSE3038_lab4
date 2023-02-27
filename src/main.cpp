#include <Arduino.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"

const char* host = "ecse-three-led-api.onrender.com";


const int ledPinNum1 = 12;
const int ledPinNum2 = 14;
const int ledPinNum3 = 27;

void setup() {
  Serial.begin(9600);
  pinMode(ledPinNum1, OUTPUT);
  pinMode(ledPinNum2, OUTPUT);
  pinMode(ledPinNum3, OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  Serial.println("");

  Serial.println("Loading");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    String http_response;


    String url = "https://ecse-three-led-api.onrender.com/api/state";
    http.begin(url);
    http.addHeader("X-API-Key", API_KEY);

    int httpResponseCode = http.GET();

    Serial.println("");
    Serial.println("");

    if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        Serial.print("Response from server: ");
        http_response = http.getString();
        Serial.println(http_response);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();

      StaticJsonDocument<1024> doc;

      DeserializationError error = deserializeJson(doc, http_response);

      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }
      
      bool LightSwitchNum1 = doc["Light_switch_1"]; 
      bool LightSwitchNum2 = doc["light_switch_2"]; 
      bool LightSwitchNum3 = doc["light_switch_3"]; 
  
      Serial.println("");
      Serial.println(LightSwitchNum1);
      Serial.println(LightSwitchNum2);
      Serial.println(LightSwitchNum3);
      digitalWrite(ledPinNum1, LightSwitchNum1);
      digitalWrite(ledPinNum2, LightSwitchNum2);
      digitalWrite(ledPinNum3, LightSwitchNum3);
      Serial.println("LED states updated");
      
  }
  else {
    return;
  }
}