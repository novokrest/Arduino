#include "PcCommunicator.h"
#include "SerialManager.h"
#include "Utils.h"
#include <StandardCplusplus.h>
#include <vector>

//EncryptedPcCommunicator communicator;

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  //Serial.begin(9600);
}

//char buf[8] = {'Q', 'W', 'E', 'R', 'T', 'Y', '0', '1'};
//char key[8] = {'0', '1', '2', '3', '4', '5', '6', '7'};
//char outbuf[8];
//
//void loop2()
//{
//  SerialManager& manager = SerialManager::Instance();
//  manager.ReadByte();
//  Utils::RepeatedBlink(5);
//}
//
//void loop3()
//{
//    std::vector<byte> vect1 = {1,2,3,4,5};
//    std::vector<byte> vect2 = {1,2,3,4,6};
//    if (vect1 == vect2) {
//        Utils::RepeatedBlink(5);
//    }
//}
//
//void reportError()
//{
//  Utils::RepeatedBlink(200);
//}
//
//void reportErrorIfFalse(bool b)
//{
//  if (!b) {
//    Utils::RepeatedBlink(200);
//  }
//}
//
//void loop4()
//{
//  std::vector<byte> data;
//  if (communicator.Read(data)) {
//    Utils::RepeatedBlink(data.size());  
//  }
//  
//}
//
//void loop5()
//{
//  const std::vector<byte> question = {'a', 'r', 'd', 'u', 'i', 'n', 'o', '?'};
//  const std::vector<byte> answer = {'y', 'e', 's'};
//  const std::vector<byte> command = {'s', 't', 'a', 'r', 't'};
//  const std::vector<byte> confirm = {'O', 'K'};
//  const std::vector<byte> fire = {'F', 'i', 'r', 'e' , '!'};
//  std::vector<byte> data;
//  
//  reportErrorIfFalse(communicator.Read(data));
//  reportErrorIfFalse(data == question);
//  communicator.Write(answer);
//
//  data.clear();
//  reportErrorIfFalse(communicator.Read(data));
//  reportErrorIfFalse(data == command);
//  communicator.Write(confirm);
//
//  communicator.Write(fire);
//}

void loop6()
{
  PcSession session;
  session.Run();
}


void loop7()
{
delay(5000);
  
DesCryptor cryptor;
PcCommunicator communicator;  
Data data = {'a', 'r', 'd', 'u', 'i', 'n', 'o', '?', '!'};
Data encrypted, decrypted;

cryptor.Encrypt(data, encrypted);
cryptor.Decrypt(encrypted, decrypted);

communicator.Open();
communicator.Write(encrypted);
communicator.Write(decrypted);

delay(10000);
}

void loop8()
{
  delay(5000);
  DesCryptor cryptor;
PcCommunicator communicator;  
Data data = {'a', 'r', 'd', 'u', 'i', 'n', 'o', '?', '!'};
Data encrypted, decrypted, received;

//cryptor.Encrypt(data, encrypted);
//cryptor.Decrypt(encrypted, decrypted);
//communicator.Write(encrypted);
//communicator.Write(decrypted);

communicator.Open();
Utils::ReportErrorIfFalse(communicator.Read(received) == 0);
communicator.Write(received);
}

void SerialTest()
{
    while (Serial.available() == 0) {} 
    byte received = Serial.read();
    Serial.write(received);   
}

void CommunicatorTest()
{
  PcCommunicator communicator;
  Data data;
  while(true) {
    communicator.Read(data);
    communicator.Write(data);
//    byte received = SerialManager::Instance().ReadByte();
//    SerialManager::Instance().WriteByte(received);
  }
}

void loop()
{
  loop6();
}


