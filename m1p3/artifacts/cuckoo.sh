#!/bin/bash

PIPE=/tmp/run/cuckoo.pid
LOG=/root/cuckoo.log

mkdir -p /tmp/run
rm -f "$PIPE"
mkfifo "$PIPE"

echo "$(date '+%Y-%m-%d %H:%M:%S') Startup!" >> "$LOG"

cleanup() {
    echo "$(date '+%Y-%m-%d %H:%M:%S') Shutdown!" >> "$LOG"
    rm -f "$PIPE"
    exit 0
}

trap cleanup SIGTERM

while true; do
    if read -r line < "$PIPE"; then
        if [[ "$line" =~ ^(.+)\[([0-9]+)\]:\ how\ much\ time\ do\ I\ have\?$ ]]; then
            NAME="${BASH_REMATCH[1]}"
            PID="${BASH_REMATCH[2]}"
            N=$(( RANDOM % 9 + 2 ))
            echo "$(date '+%Y-%m-%d %H:%M:%S') ${NAME}[${PID}] $N" >> "$LOG"
            echo "$N" > "/tmp/run/${PID}.response"
        fi
    fi
done
