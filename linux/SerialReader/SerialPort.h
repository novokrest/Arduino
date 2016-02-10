#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H


class SerialPort
{
    SerialPort() = delete;
    SerialPort(const SerialPort&) = delete;
    SerialPort(const SerialPort&&) = delete;
    SerialPort& operator=(const SerialPort&) = delete;

public:
    static int Read(int fd, char *symbol);
    static int Write(int fd, char symbol);
};

#endif // SERIAL_PORT_H
