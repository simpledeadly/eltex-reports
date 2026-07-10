#!/bin/bash

SCRIPT_NAME=$(basename "$0")
if [ "$SCRIPT_NAME" = "template_task.sh" ]; then
    echo "я бригадир, сам не работаю"
    exit 1
fi

PIPE=/tmp/run/cuckoo.pid
LOG_FILE="/root/report_${SCRIPT_NAME}.log"
PID=$$

echo "$(date '+%Y-%m-%d %H:%M:%S') [$PID] Скрипт запущен" >> "$LOG_FILE"

# Спрашиваем у cuckoo сколько у нас времени
echo "${SCRIPT_NAME}[$PID]: how much time do I have?" > "$PIPE"

# Ждём ответа (cuckoo пишет в /tmp/run/PID.response)
RESP_FILE="/tmp/run/${PID}.response"
for i in $(seq 1 20); do
    [ -f "$RESP_FILE" ] && break
    sleep 0.5
done

N=$(cat "$RESP_FILE" 2>/dev/null || echo 5)
rm -f "$RESP_FILE"

sleep "$N"

echo "$(date '+%Y-%m-%d %H:%M:%S') [$PID] Скрипт завершился, работал $N секунд" >> "$LOG_FILE"
