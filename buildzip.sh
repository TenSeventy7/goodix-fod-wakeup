#!/bin/sh
zipname=magisk-samsung-fod-wakeup.zip
cp ../libs/arm64-v8a/* system/bin/
rm $zipname
zip -r9 $zipname . -x buildzip.sh -x *.git*
