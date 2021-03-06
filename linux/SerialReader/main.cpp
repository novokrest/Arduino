#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
//#include <crypt.h>
#include <rpc/des_crypt.h>
#include <cerrno>
#include <iostream>
#include <cstring>
#include <string>
#include <exception>

static const char* ARDUINO_SERIAL_PORT = "/dev/ttyACM0";

static int OpenSerialPort(const char *serialPort)
{
    int fd = open(serialPort, O_RDWR| O_NOCTTY);
    if (fd < 0) {
        std::cout << "Failed to open arduino '" << serialPort << "': " << strerror(errno) << std::endl;
        return -1;
    }
    return fd;
}

static int ConfigureSerialPort(int fd)
{
    struct termios tty;
    struct termios tty_old;
    memset (&tty, 0, sizeof tty);

    /* Error Handling */
    if (tcgetattr (fd, &tty) != 0) {
        std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
        return -1;
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
        std::cout << "Error " << errno << " from tcsetattr" << std::endl;
        return -1;
    }
    return 0;
}

static int ReadSerialPort(int fd, char *symbol)
{
    int n = 0;
    char buf = '\0';

    n = read(fd, &buf, 1);

    if (n < 0) {
        std::cout << "Error reading: " << strerror(errno) << std::endl;
        return -1;
    }
    else if (n == 0) {
        std::cout << "Read nothing!" << std::endl;
        return 0;
    }
    *symbol = buf;
    std::cout << "Readed: " << (int)((unsigned char)buf) << std::endl;
    return 1;
}

static int ReadStartMessage(int fd, std::string startMessage)
{
    char symbol;
    for (int i = 0, len = startMessage.length(); i < len;) {
        if (ReadSerialPort(fd, &symbol) <= 0) {
            return -1;
        }
        if (symbol == startMessage.at(i)) {
            ++i;
        }
        else {
            i = 0;
        }
    }
    return 0;
}

static int ReadMessage(int fd, char buf[8])
{
    char symbol;
    ReadStartMessage(fd, "Arduino");
    for (size_t i = 0; i < sizeof(buf); ++i) {
        if (ReadSerialPort(fd, &symbol) <= 0) {
            return -1;
        }
        buf[i] = symbol;
    }
    return 0;
}

static int DecryptMessage(const char buf[8], char outbuf[8],const char key[8])
{
    for (size_t i = 0; i < sizeof(buf); ++i) {
        outbuf[i] = buf[i];
    }

    int err = ecb_crypt((char *)key, outbuf, 8, DES_DECRYPT);
    if (err == DESERR_NONE) {
        std::cout << "Blabla" << std::endl;
    }
    if (err == DESERR_NOHWDEVICE) {
        std::cout << "Blabla1" << std::endl;
    }
    if (DES_FAILED (err) == 0) {
        std::cout << "Failed to decrypt message" << std::endl;
        return -1;
    }
    else if (DES_FAILED(err) == 1) {
        std::cout << "Successfully decrypted" << std::endl;
        return 0;
    }
    return -1;
}

static int WriteSerialPortSymbol(int fd, char symbol)
{
    int attempts = 0, count;
    do {
        count = write(fd, &symbol, sizeof(symbol));
        if (count == sizeof(symbol)) {
            return 0;
        }
    }
    while(attempts++ < 3);
    return -1;
}

static int WriteStartMessage(int fd, std::string const &message)
{
    for (size_t i = 0, len = message.length(); i < len; ++i) {
        if (WriteSerialPortSymbol(fd, message.at(i)) == -1) {
            return -1;
        }
        usleep(1000);
    }
    return 0;
}

int tmain()
{
    int arduino = OpenSerialPort(ARDUINO_SERIAL_PORT);
    std::cout << "Arduino '" << ARDUINO_SERIAL_PORT << "' has been opened" << std::endl;

    if (ConfigureSerialPort(arduino) < 0) {
        return -1;
    }
    std::cout << "Arduino has been configured successfully" << std::endl;

    while(true) {
        if (WriteSerialPortSymbol(arduino, 131) < 0) {
            std::cout << "Fail" << std::endl;
        }
        else {
            //            char symbol;
            //            ReadSerialPort(arduino, &symbol);
            //std::cout << "Success: " << (int)symbol << std::endl;
        }
    }

    if (WriteStartMessage(arduino, "ubuntu") < 0) {
        std::cout << "Fail to write start message" << std::endl;
    }
    std::cout << "Start message has been sent successfully" << std::endl;

    char buf[8];
    if (ReadMessage(arduino, buf) < 0) {
        std::cout << "Failed to read encrypted message" << std::endl;
        return -1;
    }

    std::cout << "Readed message: ";
    for (size_t i = 0; i < sizeof(buf); ++i) {
        std::cout << (int)(unsigned char)buf[i] << " ";
    }
    std::cout << std::endl;

    const char key[8] = {'0', '1', '2', '3', '4', '5', '6', '7'};
    char outbuf[8];

    if (DecryptMessage(buf, outbuf, key) == -1) {
        //return -1;
    }

    std::cout << "Decrypted message: ";
    for (size_t i = 0; i < sizeof(outbuf); ++i) {
        std::cout << ( char)outbuf[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}

#include "ArduinoCommunicator.h"
#include "Keyboard.h"
#include "Tests.h"

void main1()
{
    Tests::RunTests();

    ArduinoCommunicator communicator;
    Keyboard keyboard;

    //    communicator.OpenArduinoSerial();
    //    communicator.WriteMessage("H");
    //    communicator.WriteMessage("Hello");
    //    communicator.WriteMessage("Hello");
    //    communicator.WriteMessage("Hello");
    //    communicator.WriteMessage("Hello");

    try
    {
        //keyboard.SubscribeOn(communicator);
        //communicator.Run();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

}

void main2()
{
    ArduinoSession session;
    Keyboard keyboard;

    try {
        keyboard.SubscribeOn(session);
        session.Run();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

}

void main3()
{
    ArduinoCommunicator communicator;
    Keyboard keyboard;
    Data data;

    communicator.Open();
    communicator.Read(data);

    keyboard.NotifyAboutData(data);

    DesCryptor cryptor;
    Data encrypted, decrypted;

    Data data1 = {'a', 'r', 'd', 'u', 'i', 'n', 'o', '?', '!'};
    cryptor.Encrypt(data1, encrypted);
    cryptor.Decrypt(encrypted, decrypted);


    data.clear();
    communicator.Read(data);
    keyboard.NotifyAboutData(data);
}

#include "Utils.h"
void main4()
{
    DesCryptor cryptor;
    ArduinoCommunicator communicator;
    Data data = Utils::ToData("abcdefghj");
    Data received, encrypted, decrypted;

    cryptor.Encrypt(data, encrypted);

    communicator.Open();
    communicator.Write(encrypted);
//    communicator.WriteSymbol((char)131);
//    communicator.WriteSymbol(encrypted.size());
//    for (size_t i = 0; i < encrypted.size(); ++i) {
//        communicator.WriteSymbol(encrypted.at(i));
//    }
//    communicator.WriteSymbol((char)131);
    communicator.Read(received);

    cryptor.Decrypt(encrypted, decrypted);

    int i = 0;
}

void SerialTest()
{
    ArduinoCommunicator communicator;
    char symbol, received;

    communicator.Open();
    symbol = 0;
    while(true) {
        communicator.WriteSymbol(symbol);
        communicator.ReadSymbol(&received);
        if (symbol == received) {
            std::cout << "OK: " << (int)(unsigned char)symbol << std::endl;
        }
        else {
            std::cout << "Error! Sent: " << (int)(unsigned char)symbol << ", received: " << (int)(unsigned char)received << std::endl;
        }
        ++symbol;
    }
}

void CommunicatorTest()
{
    ArduinoCommunicator communicator;
    Data data(1), received(1);
    char symbol = 0;

    communicator.Open();
    while(true) {
        data[0] = symbol;
        communicator.Write(data);
        communicator.Read(received);
        if (data == received) {
            std::cout << "OK: " << Utils::ToString(data) << std::endl;
        }
        else {
            std::cout << "Error! Sent: " << Utils::ToString(data)
                      << ", received: " << Utils::ToString(received) << std::endl;
        }
        ++symbol;
    }
}

int main()
{
    Tests::RunTests();

    //try
    {
        main2();
    }
    //catch (const std::exception& e)
    {
    //    std::cout << e.what() << std::endl;
    }

    return 0;
}

