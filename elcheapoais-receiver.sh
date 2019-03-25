#! /bin/bash

# source /etc/elcheapoais/config

while : ; do
    echo "Using device /dev/ttyUSB0..."

    stty -F /dev/ttyUSB0 38400 cs8 -cstopb -parenb
    nc -l -k -p 1222 < /dev/ttyUSB1

    echo "nc died, restarting..."
    sleep 1
done
