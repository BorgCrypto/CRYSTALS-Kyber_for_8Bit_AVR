#!/bin/sh
DEVICE=/dev/ttyACM0
DIR=`dirname $0`

avrdude -cstk500v2 -p atmega2560 -P $DEVICE -b 115200 -U flash:w:$DIR/$1  -D
stty -F $DEVICE raw icanon eof \^d 38400
cat < $DEVICE 
