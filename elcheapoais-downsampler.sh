#! /bin/bash

source /etc/elcheapoais/config

while : ; do
    LOG="/var/log/elcheapoais/downsampler.$(date +%Y-%m-%dT%H:%M).log"
    aisdownsampler server \
      --station-id "$stationid" \
      --max-message-per-sec $msgspersec \
      --max-message-per-mmsi-per-sec $msgspersecpermmsi \
      '{"type": "connect", "connect":"tcp:localhost:1221"}' \
      '{"type": "connect", "connect":"'"tcp:$server:$port"'"}' > "$LOG" 2>&1
done
