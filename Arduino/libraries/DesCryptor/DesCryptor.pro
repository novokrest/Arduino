TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += Des.cpp \
           DesCryptor.cpp \
           PcCommunicator.cpp \
    SerialManager.cpp \
    Utils.cpp \
    KeyboardManager.cpp \
    PcSession.cpp

HEADERS += Des.h \
           DesCryptor.h \
           PcCommunicator.h \
    SerialManager.h \
    Utils.h \
    KeyboardManager.h \
    PcSession.h \
    Common.h
