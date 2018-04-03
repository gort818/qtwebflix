#!/bin/bash

#script to download and extract widevine compements, tested on unbutu 17+


#Uninstall the files

if [ "$1" == "uninstall" ]; then
    echo "Uninstalling libwidevinecdmadapter.so"
    rm /usr/lib/x86_64-linux-gnu/qt5/plugins/ppapi/libwidevinecdmadapter.so
    echo "Uninstalling libwidevinecdm.so"
    rm /usr/lib/x86_64-linux-gnu/qt5/plugins/ppapi/libwidevinecdm.so
    exit
fi




#Create temp directory
tmp_dir=$(mktemp -d)
echo $tmp_dir

#set urls and checksums
url1=https://archive.archlinux.org/packages/c/chromium/chromium-61.0.3163.100-1-x86_64.pkg.tar.xz
file1=$(basename "$url1")
url2=https://dl.google.com/widevine-cdm/1.4.8.1008-linux-x64.zip
file2=$(basename "$url2")
sha256_1=("5d4380308b3d5c2bc6e13d77090688ba00f0828f2777d83376d44038a6d0e2dd")
sha256_2=("fe04a5b56eac6674f1eda2c8eb28a0183ec1a66d80f72db618291e33078eb17d")

#Download Files
wget $url1 -P $tmp_dir
wget $url2 -P $tmp_dir
#Get checksums from files
local_sha2561=$(sha256sum $tmp_dir/"$file1"| awk '{print $1}')
local_sha2562=$(sha256sum $tmp_dir/"$file2"| awk '{print $1}') 

#check checksums for first file
if [ "$sha256_1" == "$local_sha2561" ]; then
    echo "Checksums match for $file1"
else
    echo "Checksums do not match for '$file1' exiting!"
    exit
fi

#check checksums for second file

if [ "$sha256_2" == "$local_sha2562" ]; then
    echo "Checksums match for $file2"
else
    echo "Checksums do not match for '$file2' exiting!"
    exit
fi

cd $tmp_dir

#extract the files
tar -xf $tmp_dir/$file1 usr/lib/chromium/libwidevinecdmadapter.so
unzip $tmp_dir/$file2

#install the files
echo "Installing libwidevinecdmadapter.so to /usr/lib/x86_64-linux-gnu/qt5/plugins/ppapi/"
install -Dm644 $tmp_dir/usr/lib/chromium/libwidevinecdmadapter.so -t /usr/lib/x86_64-linux-gnu/qt5/plugins/ppapi/
echo "Installing libwidevinecdm.so to /usr/lib/x86_64-linux-gnu/qt5/plugins/ppapi/"
install -Dm644 $tmp_dir/libwidevinecdm.so -t /usr/lib/x86_64-linux-gnu/qt5/plugins/ppapi/







