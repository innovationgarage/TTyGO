#! /bin/bash

source /etc/elcheapoais/config

while : ; do
    echo "Using device $device..."

    stty -F "$device" $commparams
    nc localhost 1221 < "$device"

    echo "nc died, restarting..."
    sleep 1
done
