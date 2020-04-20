//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

#include <SimpleTimer.h> // Download from https://github.com/jfturcot/SimpleTimer
#include <SimpleDHT.h> // Download from https://github.com/adafruit/DHT-sensor-library

// WiFi network info.
char ssid[] = "Luftwaffe";
char wifiPassword[] = ""7758773S"";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "<20e35100-831d-11ea-883c-638d8ce4c23d>";
char password[] = "<4d0d22ac9757db688c9fb8ab5562565972bc79ba>";
char clientID[] = "<4175e4f0-8327-11ea-b767-3f1a8f1211ba>";

// DHT11 Pin
int pinDHT11 = 2;
SimpleDHT11 dht11;

// Sound Pin
int soundPin = 3;

// Relay Pin
int relayPin = 1;

// Timer
SimpleTimer timer;

void setup() {
 Serial.begin(9600);
 Cayenne.begin(username, password, clientID, ssid, wifiPassword);
 pinMode(relayPin, OUTPUT); // Relay
 digitalWrite(relayPin, HIGH);
 pinMode(soundPin, INPUT); // Sound
 timer.setInterval(200L, transmitData); // Method to execute every 200ms
}

void loop() {
 Cayenne.loop();
 timer.run();
}

CAYENNE_IN(relayPin) {
 if (getValue.asInt() == 1) { // NOTE: Channel = Virtual Pin
 digitalWrite(relayPin, LOW);
 }
 else {
 digitalWrite(relayPin, HIGH);
 }
}

void transmitData()
{
 byte temperature = 0;
 byte humidity = 0;
 int err = SimpleDHTErrSuccess;

if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
 Cayenne.virtualWrite(V4, 0);
 Cayenne.virtualWrite(V2, 0);
 }
 else {
 Cayenne.virtualWrite(V4, (int)temperature);
 Cayenne.virtualWrite(V2, (int)humidity);
 }

if (digitalRead(soundPin) == HIGH) {
 Cayenne.virtualWrite(V3, HIGH);
 }
 else {
 Cayenne.virtualWrite(V3, LOW);
 }
}
