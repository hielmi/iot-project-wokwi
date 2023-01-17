//5200411469 Hielmi Sulaeman

#include "DHTesp.h"
#include <WiFi.h>
#include "ThingSpeak.h"
#include <ESP32Servo.h>
const int DHTPin = 15;
const int relayLamp = 2;
const int relayFan = 4;
const char * ssid = "Wokwi-GUEST";
const char * pass = "";
int Temp;
int Humd;
int Lamp;
int Fan;
int pos = 0; 
// const int FieldNumber3 = 3;
// const int FieldNumber4 = 4;
unsigned long Channel = 1995924;
const char * writeAPIkey = "K1FV0XUTZY68W2XP";
Servo myservo;
WiFiClient client;
DHTesp dhtSensor;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(relayLamp, OUTPUT);
  pinMode(relayFan, OUTPUT);
  myservo.attach(5);
  dhtSensor.setup(DHTPin,DHTesp::DHT22);
  dhtSensor.getPin();
  delay(10);
  WiFi.begin(ssid,pass);
  while(WiFi.status()!=WL_CONNECTED){
    delay(100);
    Serial.print(".");
  }
  Serial.println("Terkoneksi");
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);

}

void loop() {
  delay(3000);
  
  if(Temp<=25){
    Serial.println("Suhu dingin menyalakan lampu");
    digitalWrite(relayLamp, HIGH);
  }else{
    Serial.println("suhu panas lampu dimatikan");
    digitalWrite(relayLamp, LOW);
  }
  if(Temp>=28){
    Serial.println("Suhu panas kipas dinyalakan");
    digitalWrite(relayFan, HIGH);
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  }else{
    Serial.println("Suhu panas kipas dimatikan");
    digitalWrite(relayFan, LOW);
  }
  Temp = dhtSensor.getTemperature();
  Humd = dhtSensor.getHumidity();
  Serial.println("Temperaature:");
  Serial.print(Temp);
  Serial.println("C");
  Serial.println("Humidity:");
  Serial.print(Humd);
  Serial.println("%");

  ThingSpeak.setField(1,Temp);
  ThingSpeak.setField(2,Humd);
  
  ThingSpeak.writeFields(Channel,writeAPIkey);
}
