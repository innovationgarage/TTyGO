#! /bin/sh

export PYTHONUNBUFFERED=True

mkdir -p /var/log/elcheapoais

while : ; do
    LOG="/var/log/elcheapoais/notifier.$(date +%Y-%m-%dT%H:%M).log"

    elcheapoais-notifier /etc/elcheapoais/notifier.json > "$LOG" 2>&1
done


