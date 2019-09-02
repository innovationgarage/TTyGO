#! /bin/bash

source /etc/elcheapoais/config

if [ "$stationid" == "unknown" ]; then
    echo "Finding station id using device $device..."

    stty -F "$device" $commparams
    
    mmsi="$(grep --line-buffered "AIVDO" "$device" |
              aisdecode 2> /dev/null |
              grep --line-buffered mmsi |
              head -n 1 |
              sed -e 's+.*mmsi": \([0-9]*\),.*+\1+g')"

    if cat /proc/mounts | grep "overlayroot / " > /dev/null; then
        overlayroot-chroot sed -i -e "s+stationid=.*+stationid=\"$mmsi\"+g" /etc/elcheapoais/config
    else
        sed -i -e "s+stationid=.*+stationid=\"$mmsi\"+g" /etc/elcheapoais/config
    fi
    
    reboot now
fi

while : ; do
    echo "Using device $device..."

    stty -F "$device" $commparams
    nc localhost 1221 < "$device"

    echo "nc died, restarting..."
    sleep 1
done
