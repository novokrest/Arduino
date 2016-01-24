#!/bin/bash

ARDUINO_DIR=~/Documents/Arduino
ARDUINO_LIBRARIES_DIR=$ARDUINO_DIR/libraries

ROOT=..
SCRIPT_DIR=$ROOT/Arduino/KeyboardSimulator
LIBRARIES_DIR=$ROOT/Arduino/libraries
USB_HOST_SHIELD_LIBRARY_DIR=$LIBRARIES_DIR/UsbHostShield
MY_LIBRARY_DIR=$LIBRARIES_DIR/MyLibrary

rm -r ARDUINO
cp -r $USB_HOST_SHIELD_LIBRARY_DIR