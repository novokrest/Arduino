/**
  * Based on https://github.com/felis/USB_Host_Shield_2.0/tree/master/examples/HID/USBHIDBootKbd/USBHIDBootKbd.ino
*/

#include <hidboot.h>
#include <usbhub.h>
#include <des.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

class DesCryptor
{
  typedef char DesKey[8]; 
   DesKey key_;

public:
    DesCryptor() 
    {
      DesKey key = {1,2,3,4,5,6,7,8};
      memcpy(key_, key, sizeof(DesKey));
    }

    //void Decrypt(const Data& ecnryptedData, Data& data);
    void Encrypt(uint8_t *buf, uint8_t len)
    {
      des_enc((void *)buf, (const void *)buf, (const void *)key_);
    }
};


class DecryptMouseParser : public MouseReportParser
{
  DesCryptor cryptor_;
  
  public:
    virtual void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
    {
      MouseReportParser::Parse(hid, is_rpt_id, len, buf);

      uint8_t encrypted[8];
      memcpy(encrypted, buf, 3);
      
      cryptor_.Encrypt(encrypted, 8);
      Serial.write(encrypted, 8);
      //Serial.print(len);
      //Serial.print(" ");
    }
};

USB Usb;
USBHub Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse(&Usb);

uint32_t next_time;

DecryptMouseParser Prs;

void setup()
{
  Serial.begin( 9600 );
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  //Serial.println("Start");

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  delay( 200 );

  next_time = millis() + 5000;

  HidMouse.SetReportParser(0, &Prs);
}

void loop()
{
  Usb.Task();
}

