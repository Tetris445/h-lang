#!/bin/bash
if [ -z "$1" ]; then
    echo "Использование: hc \"ваша команда\""
    exit 1
fi
TMP_FILE=$(mktemp)
echo "$1" > "$TMP_FILE"
~/h-lang/h2c "$TMP_FILE"
rm "$TMP_FILE"
