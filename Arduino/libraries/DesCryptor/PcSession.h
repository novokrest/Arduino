#ifndef PCSESSION_H
#define PCSESSION_H

#include "PcCommunicator.h"
#include "KeyboardManager.h"


class PcSession
{
    IPcCommunicator *communicator_;
    KeyboardManager *keyboardManager_;

    void ShakeHands();
    void StartLoop();

public:
    PcSession();
    ~PcSession();

    void Run();
};


#endif // PCSESSION_H
