#include "ArduinoCommunicator.h"
#include "ExceptionExtension.h"
#include "SerialPort.h"
#include "Utils.h"
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>


ArduinoCommunicator::ArduinoCommunicator()
    : serialPort_("/dev/ttyACM0"),fd_(-1)
{

}

ArduinoCommunicator::~ArduinoCommunicator()
{
    if (fd_ != -1) {
        close(fd_);
    }
}

void ArduinoCommunicator::OpenArduinoSerial()
{
    int fd = open(serialPort_.c_str(), O_RDWR| O_NOCTTY);
    if (fd < 0) {
        ExceptionExtension::ThrowErrnoExceptionWithMessage("Failed to open arduino serial port '" + serialPort_ + "'");
    }

    struct termios tty;
    struct termios tty_old;
    memset (&tty, 0, sizeof tty);

    if (tcgetattr (fd, &tty) != 0) {
        ExceptionExtension::ThrowErrnoException("tcgetattr");
    }

    /* Save old tty parameters */
    tty_old = tty;

    /* Set Baud Rate */
    cfsetospeed (&tty, (speed_t)B9600);
    cfsetispeed (&tty, (speed_t)B9600);

    /* Setting other Port Stuff */
    tty.c_cflag     &=  ~PARENB;            // Make 8n1
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;

    tty.c_lflag |= ICANON;

    tty.c_cflag     &=  ~CRTSCTS;           // no flow control
    tty.c_cc[VMIN]   =  1;                  // read doesn't block
    tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
    tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

    /* Make raw */
    cfmakeraw(&tty);


    /* Flush Port, then applies attributes */
    tcflush( fd, TCIFLUSH );
    if ( tcsetattr ( fd, TCSANOW, &tty ) != 0) {
        ExceptionExtension::ThrowErrnoException("tcsetattr");
    }

    fd_ = fd;
}

void ArduinoCommunicator::CloseArduinoSerial()
{
    if (close(fd_) == -1) {
        ExceptionExtension::ThrowErrnoException("CloseArduionoSerial");
    }
}

void ArduinoCommunicator::Open()
{
    OpenArduinoSerial();
    sleep(5);
}

void ArduinoCommunicator::ReadSymbol(char *symbol)
{
    int n = 0;
    char buf = '\0';

    n = read(fd_, &buf, 1);

    if (n <= 0) {
        ExceptionExtension::ThrowErrnoExceptionWithMessage("Failed to read symbol from arduino serial");
    }
    *symbol = buf;
    usleep(1000);
}

void ArduinoCommunicator::WriteSymbol(char symbol)
{
    int count = write(fd_, &symbol, sizeof(symbol));
    ExceptionExtension::ThrowExceptionIfFalse(count == sizeof(symbol), "Fail to write symbol");
    usleep(1000);
}

void ArduinoCommunicator::ReadData(Data& data)
{
    char symbol;
    int count;

    ReadSymbol(&symbol);
    ExceptionExtension::ThrowExceptionIfNotEqual(symbol, ARDUINO_START_DATA_KEY_SYMBOL,
                                                 "Failed to read data from arduino: Incorrect start key");

    ReadSymbol(&symbol);
    count = (int)symbol;

    data.resize(count);
    for (int i = 0; i < count; ++i) {
        ReadSymbol(&symbol);
        data[i] = symbol;
    }

    ReadSymbol(&symbol);
    ExceptionExtension::ThrowExceptionIfNotEqual(symbol, ARDUINO_END_DATA_KEY_SYMBOL,
                                                 "Failed to read data from arduino: Incorrect end key");
}

void ArduinoCommunicator::Read(Data &data)
{
    ReadData(data);
}

void ArduinoCommunicator::WriteData(const Data &data)
{
    WriteSymbol(ARDUINO_START_DATA_KEY_SYMBOL);

    WriteSymbol((char)data.size());
    for (size_t i = 0, len = data.size(); i < len; ++i) {
        WriteSymbol(data.at(i));
    }

    WriteSymbol(ARDUINO_END_DATA_KEY_SYMBOL);
}

void ArduinoCommunicator::Write(const Data &data)
{
    WriteData(data);
}


EncryptedArduinoCommunicator::EncryptedArduinoCommunicator()
    : communicator_(new ArduinoCommunicator()), cryptor_(new DesCryptor())
{

}

EncryptedArduinoCommunicator::~EncryptedArduinoCommunicator()
{
    delete communicator_;
    delete cryptor_;
}

void EncryptedArduinoCommunicator::Open()
{
    communicator_->Open();
}

void EncryptedArduinoCommunicator::Read(Data &data)
{
    Data encryptedData;

    communicator_->Read(encryptedData);
    cryptor_->Decrypt(encryptedData, data);
}

void EncryptedArduinoCommunicator::Write(const Data &data)
{
    Data encryptedData;

    cryptor_->Encrypt(data, encryptedData);
    communicator_->Write(encryptedData);
}


ArduinoSession::ArduinoSession()
    : communicator_(new EncryptedArduinoCommunicator())
{

}

ArduinoSession::~ArduinoSession()
{
    delete communicator_;
}

void ArduinoSession::WriteMessage(const std::string &message)
{
    Data data = Utils::ToData(message);
    communicator_->Write(data);
}

std::string ArduinoSession::ReadMessage()
{
    Data data;
    communicator_->Read(data);
    std::string str = Utils::ToCharString(data);
    return str;
}

void ArduinoSession::ReadAndCheckMessage(const std::string &expected)
{
    std::string received = ReadMessage();
    ExceptionExtension::ThrowExceptionIfNotEqual(expected, received, "Incorrect answer from arduino");
}

void ArduinoSession::ShakeHands()
{
    WriteMessage("arduino?");
    ReadAndCheckMessage("yes");
    WriteMessage("start");
    ReadAndCheckMessage("OK");
}

void ArduinoSession::StartLoop()
{
    while (true) {
        Data data;
        communicator_->Read(data);
        NotifyAboutData(data);
    }
}

void ArduinoSession::Run()
{
    communicator_->Open();
    ShakeHands();
    StartLoop();
}
