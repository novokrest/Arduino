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

int main()
{
    int arduino = OpenSerialPort(ARDUINO_SERIAL_PORT);
    std::cout << "Arduino '" << ARDUINO_SERIAL_PORT << "' has been opened" << std::endl;

    if (ConfigureSerialPort(arduino) < 0) {
        return -1;
    }
    std::cout << "Arduino has been configured successfully" << std::endl;

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

