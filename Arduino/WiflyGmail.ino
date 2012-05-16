// WiFly Pachube Client
// Send data to a Pachube Feed
// (Based on Ethernet's WebClient Example)
// (based upon Sparkfun WiFly Web Client example)
// Sparkfun WiFly library updated and can be found here
// https://github.com/jcrouchley/WiFly-Shield
// Built using Arduino IDE V0.22

#include <SPI.h>
#include <WiFly.h>

// using NewSoftSerial V11 beta
// downloaded from here http://arduiniana.org/2011/01/newsoftserial-11-beta/
// this will be included as Software Serial in Arduino IDE 1.0
//#include <SoftwareSerial.h>

// Wifly RN-XV (XBee shaped) module connected
//  WiFly Tx to pin 2 (SoftSerial Rx)
//  WiFly Rx to pin 3 (SoftSerial Tx)
//SoftwareSerial mySerial(4, 5);


// Edit credentials.h to provide your own credentials
#include "Credentials.h"

// Using Pachube API V2
WiFlyClient client;
int lastCount = 0;
int emailCount;

//SOUND SETUP VARIABLES-----------------------------------------------------------------
int micVal = 0;
int micPin = 0;
#define SILENT_VALUE 512  // starting neutral microphone value (self-correcting)

//LIGHT SETUP VARIABLES-----------------------------------------------------------------
int brightness = 0;    // how bright the LED is
int fadeAmount = 3;    // how many points to fade the LED by

//SWITCHES --------------------
boolean emailSwitch = false;

//First LED Strip--------------------
int RedPin1 = 12;
int GreenPin1 = 11;
int BluePin1 = 10;
//Second LED Strip--------------------
int RedPin2 = 9;
int GreenPin2 = 8;
int BluePin2 = 7;
//Third LED Strip--------------------
int RedPin3 = 6;
int GreenPin3 = 5;
int BluePin3 = 4;
//Fourth LED Strip--------------------
int RedPin4 = 46;
int GreenPin4 = 45;
int BluePin4 = 44;


void setup() {
  // lots of time for the WiFly to start up and also in case I need to stop the transmit
  delay(5000);
  
  Serial.begin(115200);  // nice and fast
  Serial1.begin(9600);  // default WiFly baud rate - good enough for this

  WiFly.setUart(&Serial1); // Tell the WiFly library that we are not using the SPIUart
  Serial.println("Wifly begin");
  panel1(0,255,0);
  delay(100);
  panel1(0,255,0);
  WiFly.begin();    // startup the WiFly
  Serial.println("Wifly join");
  // Join the WiFi network
  if (!WiFly.join(ssid, passphrase)) {
    Serial.println("Association failed.");
    while (1) {
      // Hang on failure.
    }
  }  
  pinMode(RedPin1, OUTPUT);
  pinMode(GreenPin1, OUTPUT);
  pinMode(BluePin1, OUTPUT);

}

uint32_t timeLastUpdated;
int i;
char buff[64];

void loop() {
  //THIS IS THE SECTION WHERE THE WIFLY DOES ITS THINGS------------------------------------------------------------------
  if (millis() - timeLastUpdated > TIMETOUPDATE)
  {  // time for the next update
    timeLastUpdated = millis();
    
    // prepare the data to send
    // format (API V2)
    // multiple lines each with <datastreamID>,<datastreamValue>
    // feedID can be the datastream name of the numberic ID
    //sprintf(buff,"0,%d\n1,%d",i++,analogRead(0));
    Serial.println("connecting...");
    if (client.connect("##HOSTNAME##", 80)) {
      Serial.println("connected");
      client.print("GET /wiflyGmail.php?username=########&password=########");
      client.println(" HTTP/1.1");
      client.println("Host: ##HOSTNAME##");
  
      client.println("User-Agent: Arduino (WiFly RN-XV)");
      //client.print("Content-Type: text/csv\nContent-Length: ");
      //client.println(strlen(buff));
      client.println("Connection: close");
      client.println();
  
     // client.print(buff);
      client.println();
  
    } else {
      Serial.println("connection failed");
    }

    //delay(20000);
    //SWITCH CITY------------------------------------------------------------------
    while(client.connected()){
      while (client.available()) {
        char c = client.read();
        if(c == '#'){
          superSwitch = false;
          emailSwitch = true;
          Serial.println("New Message");
        }
        else if(c == '!'){
          superSwitch = false;
          emailSwitch = false;
          Serial.println("No Message");
        }
    
      }
    }
    
   
      if (client.connected()) {
        Serial.println("disconnecting.");
        client.stop();
        Serial.println("disconnected.");
      }
    }
    //THIS IS THE SECTION WHERE THE WIFLY STOPS DOING ITS THINGS------------------------------------------------------------------
    if(emailSwitch == true){
      updown();
      updown();
      updownFinal();
    }
    else{
      panel1(0,255,50);
      panel2(0,255,50);
    }
  }

 

//THIS IS THE SECTION WHERE THE MIC DOES ITS THING (IF IN USE)------------------------------------------------------------------
int getSound(){
  static int average = SILENT_VALUE; //stores the neutral position for the mic
  static int avgEnvelope = 0; //stores the average sound pressure level
  int envSmoothing = 2; //larger values give more smoothing for envelope
  int avgSmoothing = 10; //larger values give more smoothing for the average
  int sound=analogRead(micPin); //look at voltage from eletret mic
  int envelope = abs(sound - average); // the distance from reading to average
  avgEnvelope = (envSmoothing * avgEnvelope + envelope) / (envSmoothing + 1);
  //Serial.println(avgEnvelope);
  average = (avgSmoothing * average + sound) / (avgSmoothing + 1); //create new average
  int soundVal = map(envelope, 0, 1023, 0 ,255); //scale to a single byte
  return soundVal;
}

//LIGHTING PANEL -----------------------------------------------------------------------------------
void panel1(int r, int g, int b){
  analogWrite(RedPin1, r);
  analogWrite(GreenPin1, g);
  analogWrite(BluePin1, b); 
}
void panel2(int r, int g, int b){
  analogWrite(RedPin2, r);
  analogWrite(GreenPin2, g);
  analogWrite(BluePin2, b); 
}
void panel3(int r, int g, int b){
  analogWrite(RedPin3, r);
  analogWrite(GreenPin3, g);
  analogWrite(BluePin3, b); 
}
void panel4(int r, int g, int b){
  analogWrite(RedPin4, r);
  analogWrite(GreenPin4, g);
  analogWrite(BluePin4, b); 
}

//GLOW FUNCTION -----------------------------------------------------------------------------------
void updown(){
 int counter=0;
 for(int i=0; i<255; i++){ 
    panel1(0, 255, counter);
    panel2(0, 255, counter);
    counter++;
    delay(10);
    Serial.println(counter);
   }
   for(int i=0; i<255; i++){ 
    panel1(0, 255, counter);
    panel2(0, 255, counter);
    counter--;
    delay(10);
    Serial.println(counter);
   } 
}
void updownFinal(){
 int counter=0;
 for(int i=0; i<255; i++){ 
    panel1(0, 255, counter);
    panel2(0, 255, counter);
    counter++;
    delay(10);
    Serial.println(counter);
   }
   for(int i=0; i<205; i++){ 
    panel1(0, 255, counter);
    panel2(0, 255, counter);
    counter--;
    delay(10);
    Serial.println(counter);
   } 
}
void updownCorrect(){
  int counter=255;
  for(int i=0; i<255; i++){ 
    panel1(0, counter, 50);
    panel2(0, counter, 50);
    counter--;
    delay(10);
    Serial.println(counter);
   }
}
void updownRed(){
   int counter=0;
   for(int i=0; i<255; i++){ 
    panel1(counter, 0, 50);
    panel2(counter, 0, 50);
    counter++;
    delay(10);
    Serial.println(counter);
   }
  for(int i=0; i<255; i++){ 
    panel1(counter, 0, 50);
    panel2(counter, 0, 50);
    counter--;
    delay(10);
    Serial.println(counter);
   } 
}

void updownRedFinal(){
  int counter=0;
  for(int i=0; i<255; i++){ 
    panel1(counter, 0, 50);
    panel2(counter, 0, 50);
    counter++;
    delay(10);
    Serial.println(counter);
   }
   for(int i=0; i<255; i++){ 
    panel1(counter, 0, 50);
    panel2(counter, 0, 50);
    counter--;
    delay(10);
    Serial.println(counter);
   } 
   for(int i=0; i<255; i++){
     panel1(0, counter, 50);
     panel2(0, counter, 50);
     counter++;
     delay(10); 
   }
}
