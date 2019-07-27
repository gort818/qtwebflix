# qtwebflix 

![logo](https://user-images.githubusercontent.com/8083855/50625087-08698480-0f3f-11e9-88a3-5c65a4ed9e4b.png)

A Qt webengine browser application to watch Netflix and other streaming services.

![screenshot](https://i.imgur.com/jrc7vV9.jpg) 

 **QtWebFlix on KDE Plasma Media Player and KDE Connect**
 
![kdeconnect-screenshot](https://i.imgur.com/m0NMzOK.png)

**NOTE: AS OF JULY 26 QtWebFlix can no longer stream netlifx in 1080! :( Thanks Netlifx**

**If anyone can help please feel free**

## Usage

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

![playback-rate-screenshot](https://i.imgur.com/B26CloV.png)

## News

**As of March 30th 2018:**

~~* Now supports Netflix in 1080P. NOTE not all shows and movies will work!~~
* Also supports adding other streaming services. Go to `~/.config/Qtwebflix/Providers.conf` and add your service. Example:
       
       Netflix=https://netflix.com

* To use other services right click inside the application and a context menu will bring up all available options you added.

## Instructions

### Requirements 
```
  qt5
  qtwebegine(with proprietary codecs) *Most distros have this in their repos
  widevine libs
```

If your distro does not compile qtwebengine with proprietary codecs, read [this](http://blog.qt.io/blog/2016/06/03/netflix-qt-webengine-5-7/).

### Compiling manually
 ```
  1. git clone --recursive https://github.com/gort818/qtwebflix.git
  2. cd qtwebflix
  3. qmake -config release
  4. make
  5. Binary will be labeled qtwebflix
```

### Distribution packages

#### Arch and derivatives

   Refer the [AUR package](https://aur.archlinux.org/packages/qtwebflix-git/).
   
#### Ubuntu
       
**Ubuntu 17.04/17.10/18.04 and Linux Mint 19 - qtwebengine comes with proprietary codecs**

       chmod +x widevine.sh
       sudo ./widevine.sh
       to uninstall sudo ./widevine.sh uninstall
       Follow instructions for compiling manually
       
**Ubuntu 18.10**
 
       chmod +x widevine-18.10.sh
       sudo ./widevine-18.10.sh
       to uninstall sudo ./widevine-18.10.sh uninstall      
       Follow instructions for compiling manually
       
**Note**

If you receiving an error loading qtwebflix try:

       qtwebflix --register-pepper-plugins="/usr/lib/qt/plugins/ppapi/libwidevinecdmadapter.so; application/x-ppapi-widevine-cdm"
    
### Troubleshooting 
   
**HBO Now and HBO GO are currently not working**
There have been reports that HBO Now and GO, but not verified yet.

Current workarounds:
 
 1. If you have Amazon you can add your HBO service. Read [instructions](https://help.hbogo.com/hc/en-us/articles/204872107-Watching-HBO-on-Amazon-Prime-Video-Channels).

![hbo-screenshot](https://i.imgur.com/8f7lsED.png)
