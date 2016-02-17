#include "KeyboardManager.h"
#include "Utils.h"

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


KeyboardManager::KeyboardManager()
    : numLock_(false), capsLock_(false), scrollLock_(false)
{
    lastKey_.resize(KEYBOARD_BUFFER_LENGTH);
    max_.powerOn();
    delay(1000);
}




byte KeyboardManager::InitKeyboard()
{
    epRecord_[0] = *(usb_.getDevTableEntry(0, 0));  //copy endpoint 0 parameters
    epRecord_[1].MaxPktSize = EP_MAXPKTSIZE;
    epRecord_[1].Interval  = EP_POLL;
    epRecord_[1].sndToggle = bmSNDTOG0;
    epRecord_[1].rcvToggle = bmRCVTOG0;
    usb_.setDevTableEntry(1, epRecord_);              //plug kbd.endpoint parameters to devtable

    if (usb_.setConf(KBD_ADDR, 0, 1)) {
        return -1;
    }
    if (usb_.setProto(KBD_ADDR, 0, 0, 0)) {
        return -1;
    }
    delay(2000);

    return 0;
}

/* buffer starts at position 2, 0 is modifier key state and 1 is irrelevant */
byte KeyboardManager::PollKeyboard(Keys& keys)
{
    char i;
    byte rcode = 0;
    Data buffer;

    static char leds = 0;

    buffer.resize(KEYBOARD_BUFFER_LENGTH);
    rcode = usb_.inTransfer (KBD_ADDR, KBD_EP, KEYBOARD_BUFFER_LENGTH, (char*)&buffer.front());
    if (rcode != 0) {
        return -1;
    }

    for (i = 2; i < KEYBOARD_BUFFER_LENGTH; ++i) {
        if (buffer[i] == 0) {  //end of non-empty space
            break;
        }
        if (IsNewKey(buffer[i])) {   //if new key
            switch (buffer[i]) {
                case CAPSLOCK:
                    capsLock_ =! capsLock_;
                    leds = (capsLock_) ? leds |= bmCAPSLOCK : leds &= ~bmCAPSLOCK;       // set or clear bit 1 of LED report byte
                    break;
                case NUMLOCK:
                    numLock_ =! numLock_;
                    leds = (numLock_) ? leds |= bmNUMLOCK : leds &= ~bmNUMLOCK;           // set or clear bit 0 of LED report byte
                    break;
                case SCROLLLOCK:
                    scrollLock_ =! scrollLock_;
                    leds = (scrollLock_) ? leds |= bmSCROLLLOCK : leds &= ~bmSCROLLLOCK;   // set or clear bit 2 of LED report byte
                    break;
                default:
                    break;
            }
            if (usb_.setReport( KBD_ADDR, 0, 1, KBD_IF, 0x02, 0, &leds)) {
                return -1;
            }
        }
    }

    for(i = 2; i < KEYBOARD_BUFFER_LENGTH; ++i) {                    //copy new buffer to old
        lastKey_[i] = buffer[i];
    }

    keys.push_back(buffer); /* key press */
    buffer[0] = 0;
    buffer[2] = 0;
    keys.push_back(buffer); /* key release */

    return 0;
}

bool KeyboardManager::IsNewKey(byte key)
{
    return !Utils::Contain(lastKey_, key);
}

byte KeyboardManager::WaitForKeyPress(Keys &keys)
{
    max_.Task();
    usb_.Task();

    if (usb_.getUsbTaskState() == USB_STATE_CONFIGURING) {  //wait for addressing state
        if (InitKeyboard()) {
            return -1;
        }
        usb_.setUsbTaskState(USB_STATE_RUNNING);
    }

    if (usb_.getUsbTaskState() == USB_STATE_RUNNING) {  //poll the keyboard
        if (PollKeyboard(keys)) {
            return -1;
        }
    }

    return 0;
}

