#include "ArduinoCommunicator.h"
#include "ExceptionExtension.h"
#include "SerialPort.h"
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


void ArduinoCommunicator::ShakeHands()
{
    WriteMessage("arduino?");
    ReadAndCheckMessage("yes");
    WriteMessage("start");
}

void ArduinoCommunicator::StartReceivingDataLoop()
{
    while (true) {
        Data data;
        ReadData(data);
        NotifyAboutData(data);
    }
}

void ArduinoCommunicator::Run()
{
    OpenArduinoSerial();
    ShakeHands();
    StartReceivingDataLoop();
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
    ExceptionExtension::ThrowExceptionIfFalse(count != sizeof(symbol), "Fail to write symbol");
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

    for (int i = 0; i < count; ++i) {
        ReadSymbol(&symbol);
        data.push_back(symbol);
    }

    ReadSymbol(&symbol);
    ExceptionExtension::ThrowExceptionIfNotEqual(symbol, ARDUINO_END_DATA_KEY_SYMBOL,
                                                 "Failed to read data from arduino: Incorrect end key");
}

void ArduinoCommunicator::ReadAndCheckMessage(const std::string& message)
{
    char symbol;

    ReadSymbol(&symbol);
    ExceptionExtension::ThrowExceptionIfFalse(symbol == ARDUINO_START_DATA_KEY_SYMBOL, "Failed to read start key");

    for (size_t i = 0, len = message.length(); i < len;) {
        char expected = message.at(i);
        ReadSymbol(&symbol);
        ExceptionExtension::ThrowExceptionIfFalse(symbol == message.at(i),
                                                  "Incorrect read symbol: expect '" + std::to_string(expected)
                                                  + "', actual '" + std::to_string(symbol) + "'");
    }

    ReadSymbol(&symbol);
    ExceptionExtension::ThrowExceptionIfFalse(symbol == ARDUINO_END_DATA_KEY_SYMBOL, "Failed to read end key");
}

void ArduinoCommunicator::WriteMessage(const std::string &message)
{
    WriteSymbol(ARDUINO_START_DATA_KEY_SYMBOL);

    WriteSymbol((char)message.length());
    for (size_t i = 0, len = message.length(); i < len; ++i) {
        WriteSymbol(message.at(i));
    }

    WriteSymbol(ARDUINO_END_DATA_KEY_SYMBOL);
}
