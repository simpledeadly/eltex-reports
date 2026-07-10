#!/bin/bash

CONFIG=/root/observer.conf
LOG=/root/observer.log

while IFS= read -r script || [ -n "$script" ]; do
    [ -z "$script" ] && continue
    name=$(basename "$script")
    running=0
    for pid_dir in /proc/[0-9]*/; do
        cmdline=$(cat "${pid_dir}cmdline" 2>/dev/null | tr '\0' ' ')
        if echo "$cmdline" | grep -q "$name"; then
            running=1
            break
        fi
    done

    if [ "$running" -eq 0 ]; then
        echo "$(date '+%Y-%m-%d %H:%M:%S') $name not running, starting..." >> "$LOG"
        bash "$script" &
    else
        echo "$(date '+%Y-%m-%d %H:%M:%S') $name is running OK" >> "$LOG"
    fi
done < "$CONFIG"
