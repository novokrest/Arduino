#!/bin/bash

function wait_for_keypress {
    read -n1 -rsp $'Press any key to continue or Ctrl+C to exit...\n'
}

echo "Before installing dfu-programmer"
wait_for_keypress

sudo apt-get install dfu-programmer dfu-util

echo "Before load ftdi_sio"
wait_for_keypress

sudo modprobe ftdi_sio
echo "Open this file for checking operations"
#sudo usermod -aG dialout unovokrest
#sudo chmod a+rw /dev/ttyACM0
