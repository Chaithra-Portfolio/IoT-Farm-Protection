#include <SoftwareSerial.h>

#define IR_SENSOR 7   // IR sensor connected to digital pin 7
#define RX 10         // GSM module RX to Arduino pin 10
#define TX 11         // GSM module TX to Arduino pin 11
#define buz 4

SoftwareSerial gsm(RX, TX);  // Initialize SoftwareSerial for GSM module

String phoneNumber = "+918970525674";  // Replace with the target phone number

void setup() {
    pinMode(IR_SENSOR, INPUT);
    pinMode(buz, OUTPUT);
    gsm.begin(9600);  // Initialize GSM module
    Serial.begin(9600);
    delay(1000);
    Serial.println("GSM Module Initialized");
}

void loop() {
    int sensorState = digitalRead(IR_SENSOR);
    if (sensorState == LOW) {  // Intruder detected (assuming active LOW IR sensor)
        Serial.println("Intruder detected! Sending alert...");
        digitalWrite(buz,HIGH);
        makeCall();
        delay(2000);
        sendSMS();
        delay(1000);
        digitalWrite(buz,LOW);
        delay(10000);  // Delay to prevent multiple alerts in short time
    }
}

void sendSMS() {
    
    gsm.print("AT");
    delay(1000);
    gsm.println("AT+CMGF=1");  // Set SMS mode to text
    delay(1000);
    gsm.print("AT+CMGS=\"");
    gsm.print(phoneNumber);
    gsm.println("\"");
    delay(1000);
    gsm.println("EMERGENCY Intruder detected");
    delay(1000);
    gsm.write(26);  // End SMS with Ctrl+Z
    delay(5000);
    Serial.println("SMS Sent");
}

void makeCall() {
    gsm.print("ATD");
    gsm.print(phoneNumber);
    gsm.println(";");
    Serial.println("Calling...");
    delay(30000);  // Call duration
    gsm.println("ATH");  // Hang up the call
    Serial.println("Call Ended");
}
