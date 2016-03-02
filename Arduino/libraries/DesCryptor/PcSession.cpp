#include "PcSession.h"
#include "Utils.h"


PcSession::PcSession()
    : communicator_(new EncryptedPcCommunicator()), keyboardManager_(new KeyboardManager())
{

}

PcSession::~PcSession()
{
    delete communicator_;
    delete keyboardManager_;
}

void PcSession::ShakeHands()
{
    const Data question = {'a', 'r', 'd', 'u', 'i', 'n', 'o', '?'};
    const Data answer = {'y', 'e', 's'};
    const Data command = {'s', 't', 'a', 'r', 't'};
    const Data confirm = {'O', 'K'};
    const Data fire = {'F', 'i', 'r', 'e' , '!'};

    Data data;

    Utils::ReportErrorIfFalse(communicator_->Read(data) == 0);
    Utils::ReportErrorIfFalse(data == question);
    communicator_->Write(answer);

    data.clear();
    Utils::ReportErrorIfNotZero(communicator_->Read(data));
    Utils::ReportErrorIfFalse(data == command);
    communicator_->Write(confirm);
    communicator_->Write(fire);
    delay(1000);
}

void PcSession::WaitMagicKeys()
{
//    Serial.println("Wait for magic keys");
    const Data magicKeyHids = {4, 22, 7}; // a s d
    for (byte i = 0, count = magicKeyHids.size(); i < count; ) {
//        Serial.println("Matches: ");
//        Serial.print(i);
//        Serial.println("");
        Keys keys;
        if (!keyboardManager_->WaitForKeyPress(keys) && keys.size() == 2) {
            byte pressed = keys.at(0).at(2);
            if (pressed == magicKeyHids[i]) {
                ++i;
                continue;
            }
        }
    }
}

void PcSession::StartLoop()
{
    const Data fail = {'F', 'a', 'i', 'l'};

    while(true) {
        Keys keys;
        if (keyboardManager_->WaitForKeyPress(keys)) {
            //communicator_->Write(fail);
            continue;
        }
        for (byte i = 0, len = keys.size(); i < len; ++i) {
            communicator_->Write(keys.at(i));
        }
        //keys.clear();
    }
}

void PcSession::Run()
{
    //Utils::ReportErrorIfNotZero(keyboardManager_->InitKeyboard());
    Utils::ReportErrorIfNotZero(communicator_->Open());
    //ShakeHands();
    WaitMagicKeys();
    StartLoop();
}
