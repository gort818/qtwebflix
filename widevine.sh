#!/bin/bash

mkdir  /tmp/widevine
wget -O /tmp/widevine/widevine http://ftp.us.debian.org/debian/pool/contrib/c/chromium-browser/chromium-widevine_61.0.3163.100-2_amd64.deb /tmp

cd /tmp/widevine

ar -x widevine
tar -xf data.tar.xz

wget -O google https://dl.google.com/widevine-cdm/1.4.8.1008-linux-x64.zip
unzip google

chmod 644 usr/lib/chromium/libwidevinecdmadapter.so
chmod 644 libwidevinecdm.so
mkdir -p /usr/lib/x86_64-linux-gnu/qt5/plugins/ppapi/
mv usr/lib/chromium/libwidevinecdmadapter.so /usr/lib/x86_64-linux-gnu/qt5/plugins/ppapi
mv libwidevinecdm.so /usr/lib/x86_64-linux-gnu/qt5/plugins/ppapi

