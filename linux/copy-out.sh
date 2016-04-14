#!/bin/bash

GitArduinoRoot=../Arduino
GitArduinoLibraries=$GitArduinoRoot/libraries
LinArduinoRoot=~/Arduino
LinArduinoLibraries=$LinArduinoRoot/libraries


function acopy {
  echo $1
  rm -rf $LinArduinoRoot/$1
  cp -r $GitArduinoRoot/$1 $LinArduinoRoot/$1
}

function alibcopy {
  echo $1
  rm -rf $LinArduinoLibraries/$1
  cp -r $GitArduinoLibraries/$1 $LinArduinoLibraries/$1
}

acopy 'DesCryptorUsager'
alibcopy 'DesLibrary'
alibcopy 'UsbHostShield'
alibcopy 'StandardCplusplus'

