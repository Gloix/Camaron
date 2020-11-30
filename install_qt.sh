#!/usr/bin/env bash

set -e

QT_VERSION=4.0.0

MY_PATH="`dirname \"$0\"`"              # relative
MY_PATH="`( cd \"$MY_PATH\" && pwd )`"  # absolutized and normalized
if [ -z "$MY_PATH" ] ; then
  # error; for some reason, the path is not accessible
  # to the script (e.g. permissions re-evaled after suid)
  exit 1  # fail
fi
QT_INSTALLER_PATH="$MY_PATH/.temp/qt_installer.run"

mkdir -p "$MY_PATH/.temp"

read -p 'QT account email: ' QT_EMAIL
read -s -p 'QT account password: ' QT_PASS

wget -O $QT_INSTALLER_PATH https://d13lb3tujbc8s0.cloudfront.net/onlineinstallers/qt-unified-linux-x64-$QT_VERSION-online.run

chmod +x $QT_INSTALLER_PATH

$QT_INSTALLER_PATH --accept-licenses --default-answer --email "$QT_EMAIL" --password "$QT_PASS" --accept-obligations --confirm-command install