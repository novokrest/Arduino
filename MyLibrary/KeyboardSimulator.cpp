#include "ArduinoDefines.h"
#include "KeyboardSimulator.h"

/* keyboard data taken from configuration descriptor */
#define KBD_ADDR        1
#define KBD_EP          1
#define KBD_IF          0
#define EP_MAXPKTSIZE   8
#define EP_POLL         0x0a
/**/
//******************************************************************************
//  macros to identify special charaters(other than Digits and Alphabets)
//******************************************************************************
#define BANG        (0x1E)
#define AT          (0x1F)
#define POUND       (0x20)
#define DOLLAR      (0x21)
#define PERCENT     (0x22)
#define CAP         (0x23)
#define AND         (0x24)
#define STAR        (0x25)
#define OPENBKT     (0x26)
#define CLOSEBKT    (0x27)

#define RETURN      (0x28)
#define ESCAPE      (0x29)
#define BACKSPACE   (0x2A)
#define TAB         (0x2B)
#define SPACE       (0x2C)
#define HYPHEN      (0x2D)
#define EQUAL       (0x2E)
#define SQBKTOPEN   (0x2F)
#define SQBKTCLOSE  (0x30)
#define BACKSLASH   (0x31)
#define SEMICOLON   (0x33)
#define INVCOMMA    (0x34)
#define TILDE       (0x35)
#define COMMA       (0x36)
#define PERIOD      (0x37)
#define FRONTSLASH  (0x38)
#define DELETE      (0x4c)
/**/
/* Modifier masks. One for both modifiers */
#define SHIFT       0x22
#define CTRL        0x11
#define ALT         0x44
#define GUI         0x88
/**/
/* "Sticky keys */
#define CAPSLOCK    (0x39)
#define NUMLOCK     (0x53)
#define SCROLLLOCK  (0x47)
/* Sticky keys output report bitmasks */
#define bmNUMLOCK       0x01
#define bmCAPSLOCK      0x02
#define bmSCROLLLOCK    0x04
/**/

KeyboardSimulator::KeyboardSimulator()
{
    KeyboardInit();
    _numLock = false;
    _capsLock = false;
    _scrollLock = false;
}

/* Initialize keyboard */
void KeyboardSimulator::KeyboardInit()
{
    EP_RECORD ep_record[ 2 ];  //endpoint record structure for the keyboard
    byte rcode = 0;
    
    ep_record[ 0 ] = *(_usb.getDevTableEntry( 0,0 ));  //copy endpoint 0 parameters
    ep_record[ 1 ].MaxPktSize = EP_MAXPKTSIZE;
    ep_record[ 1 ].Interval  = EP_POLL;
    ep_record[ 1 ].sndToggle = bmSNDTOG0;
    ep_record[ 1 ].rcvToggle = bmRCVTOG0;
    _usb.setDevTableEntry( 1, ep_record );              //plug kbd.endpoint parameters to devtable
    
    rcode = _usb.setConf( KBD_ADDR, 0, 1 );
    if (rcode) {
        while( 1 ) { BlinkError(); };  //stop
    }
    
    rcode = _usb.setProto( KBD_ADDR, 0, 0, 0 );
    if (rcode) {
        while( 1 ) { BlinkError(); };  //stop
    }
    
    delay(2000);
}

/* Poll keyboard and print result */
/* buffer starts at position 2, 0 is modifier key state and 1 is irrelevant */
/* return 0 on succes, 1 - on fail */
int KeyboardSimulator::KeyboardPoll()
{
    char i;
    char buffer[8];
    static char leds = 0;
    byte rcode = 0;
    
    rcode = _usb.inTransfer (KBD_ADDR, KBD_EP, 8, buffer);
    if (rcode) {
        return rcode;
    }
    
    rcode = _usb.setReport (KBD_ADDR, 0, 1, KBD_IF, 0x02, 0, &leds);
    if (rcode) {
        return rcode;
    }
    for (i = 2; i < 8; i++) {
        if (buffer[i] == 0) {  //end of non-empty space
            break;
        }
        if (IsNewKey(buffer[i]) == false ) {   //if new key
            switch (buffer[i]) {
                case CAPSLOCK:
                    _capsLock =! _capsLock;
                    leds = ( _capsLock ) ? leds |= bmCAPSLOCK : leds &= ~bmCAPSLOCK;       // set or clear bit 1 of LED report byte
                    break;
                case NUMLOCK:
                    _numLock =! _numLock;
                    leds = ( _numLock ) ? leds |= bmNUMLOCK : leds &= ~bmNUMLOCK;           // set or clear bit 0 of LED report byte
                    break;
                case SCROLLLOCK:
                    _scrollLock =! _scrollLock;
                    leds = ( _scrollLock ) ? leds |= bmSCROLLLOCK : leds &= ~bmSCROLLLOCK;   // set or clear bit 2 of LED report byte
                    break;
                default:
                    break;
            }
            rcode = _usb.setReport( KBD_ADDR, 0, 1, KBD_IF, 0x02, 0, &leds );
            if( rcode ) {
                return rcode;
            }
        }
    }
    
    for( i = 2; i < 8; i++ ) {                    //copy new buffer to old
        _lastKeys[i] = buffer[i];
    }
}

void KeyboardSimulator::BlinkError()
{
    _blinker.blink();
}

bool KeyboardSimulator::IsNewKey(byte key)
{
    char i;
    for (i = 2; i < 8; i++) {
        if (_lastKeys[i] == key) {
            return true;
        }
    }
    return false;

}

void KeyboardSimulator::Run()
{
    while (true) {
        ReadPressedKeys();
        SendPressedKeys();
    }
}

void KeyboardSimulator::ReadPressedKeys()
{
    byte status;
    do {
        status = KeyboardPoll();
        if (status) BlinkError();
    } while (status);
    
}

void KeyboardSimulator::SendPressedKeys()
{
    char buffer[8];
    
    memcpy(buffer, _lastKeys, 8);
    Serial.write(buffer, sizeof(_lastKeys)); // Send keypress
    
    buffer[0] = 0;
    buffer[2] = 0;
    Serial.write(buffer, sizeof(buffer)); // Release key
}



