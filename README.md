# qtwebflix
A qt webengine program to watch Netflix and other streaming services


![alt text](https://i.imgur.com/jrc7vV9.jpg) 


<b> Usage </b>
```
Usage: qtwebflix [options]
Qtwebflix Help

 Shortcuts:
 CTRL + Q to quit
 CTRL + F11 for full screen
 CTRL + F5 to reload
 CTRL + SHIFT + ALT + D for metrics display

 To Control playback rate :
 CTRL + W = Speed up 
 CTRL + S = Slow down 
 CTRL + R = Reset to default rate

Options:
  -h, --help                   Displays this help.
  -v, --version                Displays version information.
  -p, --provider <provider>    Set content provider eg. netflix.com
  -u, --useragent <useragent>  change useragent eg. "Mozilla/5.0 (X11; Linux
                               x86_64; rv:63.0) Gecko/20100101 Firefox/63.0"
  -n, --nonhd                  Do not use HD addon, you will be limited to 720p
```


Example of playback rate visualizer.
![alt text](https://i.imgur.com/B26CloV.png)
<b>As of March 30th 2018:</b>

       Now supports Netflix in 1080P.
       Also supports adding other streaming services go to ~/.config/Qtwebflix/Providers.conf and add your service
       ex. netflix=https://netflix.com
       To use these other services right clicking will bring up a context menu with all available options.

<b>Now available in the AUR</b>


<b>Requirements-</b> qt5,qtwebegine(with proprietary codecs), widvine libs


If your distro does not compile qtwebengine with proprietary codecs.

Read this <html>http://blog.qt.io/blog/2016/06/03/netflix-qt-webengine-5-7/</html>

<h1>Instructions:</h1>


<b>Arch and derivatives</b>

   Refer the aur package <html>https://aur.archlinux.org/packages/qtwebflix-git/</html>
      
       
 <b>Ubuntu 17.04/17.10/18.04 and Linux Mint 19 - qtwebengine comes with proprietary codecs</b>

       chmod +x widevine.sh
       sudo ./widevine.sh
       to uninstall sudo ./widevine.sh uninstall
       Follow instructions for compiling manually
       
<b> Ubuntu 18.10</b>
 
       chmod +x widevine-18.10.sh
       sudo ./widevine-18.10.sh
       to uninstall sudo ./widevine-18.10.sh uninstall      
       Follow instructions for compiling manually
       
<b>Compile manually</b>
  1. Install qt5
  2. Compile qtwebengine according to the link above
  3. Dowload the source
  4. cd into the folder
  5. Type make clean
  6. Type qmake -config release
  7. Type make
  8. Binary will be labeled qtwebflix
  9. Enjoy a standalone netflix player :) f11 for fullscreen and crtl + q to quit
  
Note:
If you receiving an error loading qtwebflix try


       qtwebflix --register-pepper-plugins="/usr/lib/qt/plugins/ppapi/libwidevinecdmadapter.so; application/x-ppapi-widevine-cdm"
    
    
   
<b> HBO Now and HBO GO are currently not working </b>

 Current workarounds :
 
 1. If you have amazon you can add your HBO service.
[instructions](https://help.hbogo.com/hc/en-us/articles/204872107-Watching-HBO-on-Amazon-Prime-Video-Channels)

![alt text](https://i.imgur.com/8f7lsED.png)


