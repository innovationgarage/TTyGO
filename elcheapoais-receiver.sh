#! /bin/bash

source /etc/elcheapoais/config

while : ; do
    echo "Using device $device..."

    stty -F "$device" $commparams
    nc -l -k -p 1222 < "$device"

    echo "nc died, restarting..."
    sleep 1
done
