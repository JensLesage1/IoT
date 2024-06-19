#include <ArduinoMqttClient.h>  
#include "WiFiS3.h"
#include "wifiSecret.h" // you need to create this file with your own network credentials (in .gitignore)

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = WOT;        // your network SSID (name)
char pass[] = enterthegame;    // your network password (use for WPA, or use as key for WEP)
 
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
 
const char broker[] = "192.168.50.46";
const int port     = 1883;
const char topic[]  = "arduino/jannesiscool";

//set interval for sending messages (milliseconds)
const long interval = 1000;
unsigned long previousMillis = 0;
 
 // Define the pin numbers for the IR sensors
const int irSensorPin1 = 2; // Right sensor
const int irSensorPin2 = 3; // Left sensor

// Define variables to keep track of counts
int inCount = 0;   // Number of people entering the store
int outCount = 0;  // Number of people exiting the store
int currentCount = 0; // Number of people currently in the store
int count = 0;

// Define variables to track the state of each sensor
bool rightSensorState = false;
bool leftSensorState = false;

// Define variables to track previous states of sensors
bool prevRightSensorState = false;
bool prevLeftSensorState = false;

// Define variables to debounce sensor readings
unsigned long debounceDelay = 50; // milliseconds
unsigned long lastDebounceTime1 = 0; // Right sensor
unsigned long lastDebounceTime2 = 0; // Left sensor

// Define minimum time a sensor should be triggered to count as a valid trigger
unsigned long minTriggerTime = 1000; // milliseconds
unsigned long triggerStartTime = 0;

// Define cooldown period after each count
unsigned long cooldownPeriod = 2000; // milliseconds
unsigned long lastCountTime = 0;


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
 
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
 
  Serial.println("You're connected to the network");
  Serial.println();
 
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
 
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
 
    while (1);
  }
 
  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  // Set the IR sensor pins as inputs with internal pull-up resistors enabled
  pinMode(irSensorPin1, INPUT_PULLUP);
  pinMode(irSensorPin2, INPUT_PULLUP);
}


void loop() {
  // Read the state of the IR sensors
  rightSensorState = digitalRead(irSensorPin1);
  leftSensorState = digitalRead(irSensorPin2);
  
  // Debounce sensor readings
  if ((millis() - lastDebounceTime1) > debounceDelay) {
    // Update right sensor state if it's changed
    if (rightSensorState != prevRightSensorState) {
      prevRightSensorState = rightSensorState;
      
      // Check if a person is entering the store
      if (rightSensorState && !leftSensorState) {
        triggerStartTime = millis();
      } else if (rightSensorState && leftSensorState && (millis() - triggerStartTime >= minTriggerTime)) {
        if (millis() - lastCountTime >= cooldownPeriod) {
          inCount++;
          currentCount++;
          lastCountTime = millis();
        }
      }
    }
    
    lastDebounceTime1 = millis();
  }
  
  if ((millis() - lastDebounceTime2) > debounceDelay) {
    // Update left sensor state if it's changed
    if (leftSensorState != prevLeftSensorState) {
      prevLeftSensorState = leftSensorState;
      
      // Check if a person is exiting the store
      if (leftSensorState && !rightSensorState) {
        triggerStartTime = millis();
      } else if (leftSensorState && rightSensorState && (millis() - triggerStartTime >= minTriggerTime)) {
        if (millis() - lastCountTime >= cooldownPeriod) {
          outCount++;
          if (currentCount > 0) {
            currentCount--;
          }
          lastCountTime = millis();
        }
      }
    }
    
    lastDebounceTime2 = millis();
  }
  
  // Print counts to the serial monitor
  Serial.print("People In: ");
  Serial.print(inCount);
  Serial.print("\tPeople Out: ");
  Serial.print(outCount);
  Serial.print("\tCurrent Count: ");
  Serial.println(currentCount);
  mqttClient.poll();
 
  unsigned long currentMillis = millis();a
 
  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;
 
    int numberToSend = inCount;  // Or outCount or currentCount, depending on which value you want to send
    String stringOne = "Hello";                  
                  
 
    Serial.print("Sending message to topic: ");
    Serial.println(stringOne);
    Serial.println(numberToSend);
 
 
    // send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(topic);
    mqttClient.print(stringOne);
    mqttClient.endMessage();
 
    Serial.println();
  }

 
  
}
