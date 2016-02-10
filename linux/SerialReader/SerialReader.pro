TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += c++11

SOURCES += main.cpp \
    DesCryptor.cpp \
    ArduinoCommunicator.cpp \
    Keyboard.cpp \
    Arduino.cpp \
    Utils.cpp \
    Tests.cpp \
    DataObserver.cpp \
    ExceptionExtension.cpp \
    SerialPort.cpp

HEADERS += \
    DesCryptor.h \
    ArduinoCommunicator.h \
    Keyboard.h \
    Arduino.h \
    common.h \
    Utils.h \
    Tests.h \
    DataObserver.h \
    ExceptionExtension.h \
    SerialPort.h

