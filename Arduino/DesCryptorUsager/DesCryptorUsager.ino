#include "DesCryptor.h"

DesCryptor cryptor;

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}

char buf[8] = {'Q', 'W', 'E', 'R', 'T', 'Y', '0', '1'};
char key[8] = {'0', '1', '2', '3', '4', '5', '6', '7'};
char outbuf[8];

void SendMessage()
{
  
  cryptor.Encrypt(buf, outbuf, key);
  //cryptor.Decrypt(outbuf, buf, key);

  String s("Arduino");
  Serial.write(s.c_str(), s.length());
  Serial.write(outbuf, 8);

  //String s(1145);
  //Serial.write(s.c_str(), s.length());
}

void loop() {
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  SendMessage();
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
}

