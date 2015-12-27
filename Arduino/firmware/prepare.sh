#!/bin/bash

function read_answer {
    ANSWER=""
    read -n 1 ANSWER
    if [ "$ANSWER" = "q" ]; then
        echo
        exit
    fi
}

function install_firmware {
    if [ "$#" -ne 1 ]; then
        echo "Illegal number of parameters"
        exit
    fi

    CHIP=atmega16u2
    FIRMWARE=$1

    #echo $CHIP
    #echo $FIRMWARE

    sudo dfu-programmer $CHIP erase
    sudo dfu-programmer $CHIP flash --debug 1 $FIRMWARE
    sudo dfu-programmer $CHIP reset
}

ROOT=.
ORIGINAL_FIRMWARE=$ROOT/Arduino-usbserial-uno.hex
KEYBOARD_FIRMWARE=$ROOT/Arduino-keyboard-0.3.hex

echo "Go to DFU mode and press ENTER [press 'q' for exit]"
read_answer

echo "Press ENTER for installing original firmware [press 'q' for exit]"
read_answer
install_firmware $ORIGINAL_FIRMWARE
echo "Original firmware has been installed."; echo

echo "Upload your sketch and press ENTER for continue [press 'q' for exit]"
read_answer

echo "Go to DFU mode and press ENTER [press 'q' for exit]"
read_answer

echo "Press ENTER for installing keyboard firmware [press 'q' for exit]"
read_answer
install_firmware $KEYBOARD_FIRMWARE
echo "Keyboard firmware has been installed."; echo
