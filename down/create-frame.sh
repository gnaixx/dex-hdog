#!/usr/bin/env bash

adb root

adb push dexopt-wrapper /data/local/tmp

adb shell chmod 777 /system/bin

adb shell cp /data/local/tmp/dexopt-wrapper /system/bin

adb shell chmod 777 /system/bin/dexopt-wrapper

echo "for dir in /system/framework/*; do dexopt-wrapper \${dir} \${dir%.*}.odex; done" > run.sh

adb push run.sh /data/local/tmp/

adb shell chmod 777 /data/local/tmp/run.sh

adb shell ./data/local/tmp/run.sh

rm run.sh