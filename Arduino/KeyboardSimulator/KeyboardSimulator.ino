/* MAX3421E USB Host controller LCD/keyboard demonstration */
#include <Max3421e.h>
#include <Usb.h>

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
EP_RECORD ep_record[ 2 ];  //endpoint record structure for the keyboard

char buf[ 8 ] = { 0 };      //keyboard buffer
char old_buf[ 8 ] = { 0 };  //last poll
/* Sticky key state */
bool numLock = false;
bool capsLock = false;
bool scrollLock = false;
bool line = false;

void setup();
void loop();
void kbd_init();
void kbd_poll();
bool buf_compare( byte data );
byte HIDtoA( byte HIDbyte, byte mod );
void blink_error();
void releaseKey();

MAX3421E Max;
USB Usb;

void setup() 
{
  pinMode(13, OUTPUT);
  Serial.begin( 9600 );
  Max.powerOn();
  delay( 200 );
}

void loop() 
{
    Max.Task();
    Usb.Task();
    if( Usb.getUsbTaskState() == USB_STATE_CONFIGURING ) {  //wait for addressing state
        kbd_init();
        Usb.setUsbTaskState( USB_STATE_RUNNING );
    }
    if( Usb.getUsbTaskState() == USB_STATE_RUNNING ) {  //poll the keyboard  
        kbd_poll();
    }
}

/* Initialize keyboard */
void kbd_init() 
{
    byte rcode = 0;

    ep_record[ 0 ] = *( Usb.getDevTableEntry( 0,0 ));  //copy endpoint 0 parameters
    ep_record[ 1 ].MaxPktSize = EP_MAXPKTSIZE;
    ep_record[ 1 ].Interval  = EP_POLL;
    ep_record[ 1 ].sndToggle = bmSNDTOG0;
    ep_record[ 1 ].rcvToggle = bmRCVTOG0;
    Usb.setDevTableEntry( 1, ep_record );              //plug kbd.endpoint parameters to devtable

    rcode = Usb.setConf( KBD_ADDR, 0, 1 );                    
    if( rcode ) {
        while( 1 ) { blink_error(); };  //stop
    }

    rcode = Usb.setProto( KBD_ADDR, 0, 0, 0 );
    if( rcode ) {
        while( 1 ) { blink_error(); };  //stop
    }
    
    delay(2000);
}

/* Poll keyboard and print result */
/* buffer starts at position 2, 0 is modifier key state and 1 is irrelevant */
void kbd_poll()
{
    char i;
    static char leds = 0;
    byte rcode = 0;

    rcode = Usb.inTransfer( KBD_ADDR, KBD_EP, 8, buf );
    if( rcode ) {
        return;
    }
    
    rcode = Usb.setReport( KBD_ADDR, 0, 1, KBD_IF, 0x02, 0, &leds );
    if( rcode ) {
      return;
    }
    for( i = 2; i < 8; i++ ) {
      if( buf[ i ] == 0 ) {  //end of non-empty space
        break;
      }
      if( buf_compare( buf[ i ] ) == false ) {   //if new key
        switch( buf[ i ] ) {
          case CAPSLOCK:
            capsLock =! capsLock;
            leds = ( capsLock ) ? leds |= bmCAPSLOCK : leds &= ~bmCAPSLOCK;       // set or clear bit 1 of LED report byte
            break;
          case NUMLOCK:
            numLock =! numLock;
            leds = ( numLock ) ? leds |= bmNUMLOCK : leds &= ~bmNUMLOCK;           // set or clear bit 0 of LED report byte
            break;
          case SCROLLLOCK:
            scrollLock =! scrollLock;
            leds = ( scrollLock ) ? leds |= bmSCROLLLOCK : leds &= ~bmSCROLLLOCK;   // set or clear bit 2 of LED report byte
            break;  
          default:
            break;
        }
        rcode = Usb.setReport( KBD_ADDR, 0, 1, KBD_IF, 0x02, 0, &leds );
        if( rcode ) {
          blink_error();
        }
      }
    }
    
    for( i = 2; i < 8; i++ ) {                    //copy new buffer to old
      old_buf[ i ] = buf[ i ];
    }

    Serial.write(buf, 8); // Send keypress
    releaseKey();
}

void releaseKey() 
{
  buf[0] = 0;
  buf[2] = 0;
  Serial.write(buf, 8); // Release key  
}

bool buf_compare( byte data )
{
 char i;
 for( i = 2; i < 8; i++ ) {
   if( old_buf[ i ] == data ) {
     return( true );
   }
 }
 return( false );
}

void blink_error()
{
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);  
}

