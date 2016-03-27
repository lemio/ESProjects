/*
Sending random remote commands 
ESP   433(TX)
GND - GND
D8 - DATA
VCC -  5V
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(9600);
  
  // Transmitter is connected to nodeMCU pin D8
  mySwitch.enableTransmit(11);
  
  //Setting pins for plugnplay
  
  pinMode(10,OUTPUT);
  digitalWrite(10,HIGH);
  pinMode(9,OUTPUT);
  digitalWrite(9,LOW);
  // Optional set pulse length.
  // mySwitch.setPulseLength(320);
  
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);
  
  // Optional set number of transmission repetitions.
  //mySwitch.setRepeatTransmit(1);
  
}

void loop() {

  
  mySwitch.send(analogRead(0) + 1000, 24);
  delay(1000);  

  

}
