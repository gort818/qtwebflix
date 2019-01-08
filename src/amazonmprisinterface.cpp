#include <QDebug>
#include <QWebEngineView>
#include <QWidget>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "mainwindow.h"
#include "mprisinterface.h"
#include "amazonmprisinterface.h"

AmazonMprisInterface::AmazonMprisInterface(QWidget *parent)
    : MprisInterface(parent) {
  prevTitleId = "";
  prevArtUrl = "";
  titleInfoFetching = false;
}

void AmazonMprisInterface::setup(MainWindow *window) {
  MprisInterface::setup(window);

  workWithPlayer([this] (MprisPlayer& p) {
    // Expose player capabilities.
    p.setCanQuit(true);
    p.setCanSetFullscreen(true);
    p.setCanPause(true);
    p.setCanPlay(true);
    p.setCanControl(true);
    p.setCanSeek(true);

    p.setMetadata(QVariantMap());

    connect(&p, SIGNAL(pauseRequested()), this, SLOT(pauseVideo()));
    connect(&p, SIGNAL(playRequested()), this, SLOT(playVideo()));
    connect(&p, SIGNAL(playPauseRequested()), this, SLOT(togglePlayPause()));
    connect(&p, SIGNAL(fullscreenRequested(bool)), this, SLOT(setFullScreen(bool)));
    connect(&p, SIGNAL(volumeRequested(double)), this, SLOT(setVideoVolume(double)));
    connect(&p, SIGNAL(seekRequested(qlonglong )), this, SLOT(setPosition(qlonglong )));

  });


  // Connect slots and start timers.
  connect(&playerStateTimer, SIGNAL(timeout()), this, SLOT(playerStateTimerFired()));
  playerStateTimer.start(500);

  connect(&playerPositionTimer, SIGNAL(timeout()), this, SLOT(playerPositionTimerFired()));
  playerPositionTimer.start(170);

  connect(&metadataTimer, SIGNAL(timeout()), this, SLOT(metadataTimerFired()));
  metadataTimer.start(1000);

  connect(&volumeTimer, SIGNAL(timeout()), this, SLOT(volumeTimerFired()));
  volumeTimer.start(220);
}

void AmazonMprisInterface::playVideo() {
  QString code = ("(function () {" \
                  "var vid =  document.querySelectorAll('video');" \
                  "for (i = 0; i < vid.length; ++i) { if(vid[i].getAttribute('src')) {var video = vid[i]} };"\
                  "if (video == undefined) return;" \
                  "video.play();" \
                   "})();");
  qDebug() << "Player playing";
  webView()->page()->runJavaScript(code);
}

void AmazonMprisInterface::pauseVideo() {
  QString code = ("(function () {" \
                  "var vid =  document.querySelectorAll('video');" \
                  "for (i = 0; i < vid.length; ++i) { if(vid[i].getAttribute('src')) {var video = vid[i]} };"\
                  "if (!video) return;" \
                  "video.pause();" \
                  "})();");
  qDebug() << "Player paused";
  webView()->page()->runJavaScript(code);
}

void AmazonMprisInterface::togglePlayPause() {
  QString code = ("(function () {" \
                  "var vid =  document.querySelectorAll('video');" \
                  "for (i = 0; i < vid.length; ++i) { if(vid[i].getAttribute('src')) {var video = vid[i]} };"\
                  "if (video.paused) video.play();" \
                  "else video.pause();" \
                  "})();");
  qDebug() << "Player toggled play/pause";
  webView()->page()->runJavaScript(code);
}

void AmazonMprisInterface::setVideoVolume(double volume) {
  QString code = ("(function () {" \
                  "var vid =  document.querySelectorAll('video');" \
                  "for (i = 0; i < vid.length; ++i) { if(vid[i].getAttribute('src')) {var video = vid[i]} };"\
                  "if (!video) return;" \
                  "video.volume = " + QString::number(volume) + ";" \
                 "})();");
  qDebug() << "Player set volume to " << volume;
  webView()->page()->runJavaScript(code);
}

void AmazonMprisInterface::setFullScreen(bool fullscreen) {
  window()->setFullScreen(fullscreen);
}

void AmazonMprisInterface::getVolume(std::function<void(double)> callback) {
  QString code = ("(function () {" \
                  "var vid =  document.querySelectorAll('video');" \
                  "for (i = 0; i < vid.length; ++i) { if(vid[i].getAttribute('src')) {var video = vid[i]} };"\
                 "return video ? video.volume : -1;" \
                 "})()");
  webView()->page()->runJavaScript(code, [callback](const QVariant& result) {
    callback(result.toDouble());
  });
}

void AmazonMprisInterface::getVideoPosition(std::function<void(qlonglong)> callback) {
  QString code = ("(function () {" \
                  "var vid =  document.querySelectorAll('video');" \
                  "for (i = 0; i < vid.length; ++i) { if(vid[i].getAttribute('src')) {var video = vid[i]} };"\
                 "return video ? video.currentTime -10: -1;" \
                 "})()");
  webView()->page()->runJavaScript(code, [callback](const QVariant& result) {
    double seconds = result.toDouble();
    if (seconds < 0) callback(-1);
    else callback(seconds / 1e-6);
  });
}


void AmazonMprisInterface::setPosition(qlonglong pos){
    double seconds = static_cast<double>(pos);
    //double useconds= seconds/1e+6;
    double useconds = seconds /1e+6;
    qDebug()<<"Seeking Position by " <<useconds <<" Seconds";
   QString code = ("(function () {" \
                   "var vid =  document.querySelectorAll('video');" \
                   "for (i = 0; i < vid.length; ++i) { if(vid[i].getAttribute('src')) {var video = vid[i]} };"\
                   "if (!video) return;" \
                   "video.pause();"\
                   "video.currentTime += " + QString::number(useconds) + ";" \
                   "var timer = setInterval(function() {"\
                   "if (video.paused && video.readyState ==4 || !video.paused) {"\
                   "video.play();"\
                   "clearInterval(timer);"\
                   "}"\
                   "}, 50);"\
                   "})();");
   webView()->page()->runJavaScript(code);
}

void AmazonMprisInterface::getMetadata(std::function<void(qlonglong, const QString&, const QString&, const QString&)> callback) {
  QString code = ("(function () {" \
                 "var vid = document.querySelectorAll('video');" \
                 "for (i = 0; i < vid.length; ++i) { if(vid[i].getAttribute('src')) {var video = vid[i]} };"\
                 "var  titleLabel =document.querySelector('div.title').innerText;" \
                 "var subLabel  =document.querySelector('div.subtitle').innerText;"\
                 "var metadata = {};"\
                 "metadata.duration = video ? video.duration : -1;" \
                 "metadata.nid = video && video.offsetParent ? video.offsetParent.id : '';" \
                 "metadata.title = titleLabel + subLabel;"\
                 "try { var art =document.querySelector('div.av-bgimg__div').getAttribute('style').split('url')[1].replace('(','').replace(')','');"\
                 "}catch (err ) {}"\
                 "finally {"\
                 "try{ var art = document.querySelector('div.av-fallback-packshot').childNodes[0].getAttribute('src');"\
                 "} catch(err ) {} };"\
                 "metadata.arturl= art;"\
                 "return metadata;"
          "})()");
  webView()->page()->runJavaScript(code, [callback](const QVariant& result) {
    QVariantMap map = result.toMap();

    double seconds = map["duration"].toDouble();
    if (seconds < 0) seconds = -1;
    else seconds /= 1e-6;

    QString title = map["title"].toString();
    QString nid = map["nid"].toString();
    QString artUrl =map["arturl"].toString();

    callback(seconds, title, nid,artUrl);
  });
}

void AmazonMprisInterface::getVideoState(std::function<void(Mpris::PlaybackStatus)> callback) {
  QString code = ("(function () {" \
                  "var vid =  document.querySelectorAll('video');" \
                  "for (i = 0; i < vid.length; ++i) { if(vid[i].getAttribute('src')) {var video = vid[i]} };"\
                 "if (!video) return 'stopped';" \
                 "return video.paused ? 'paused' : 'playing';" \
                 "})()");
  webView()->page()->runJavaScript(code, [callback](const QVariant& result) {
    QString resultString = result.toString();
    Mpris::PlaybackStatus status = Mpris::InvalidPlaybackStatus;
    if (resultString == "stopped") status = Mpris::Stopped;
    else if (resultString == "playing") status = Mpris::Playing;
    else if (resultString == "paused") status = Mpris::Paused;
    callback(status);
  });
}

void AmazonMprisInterface::playerStateTimerFired() {
  getVideoState([this](Mpris::PlaybackStatus state) {
    workWithPlayer([&] (MprisPlayer& p) {
      p.setPlaybackStatus(state);
    });
  });
}

void AmazonMprisInterface::playerPositionTimerFired() {
  getVideoPosition([this](qlonglong useconds) {
    workWithPlayer([&] (MprisPlayer& p) {
      p.setPosition(useconds);
    });
  });
}

void AmazonMprisInterface::metadataTimerFired() {
  getMetadata([this] (qlonglong lengthUseconds, const QString& title, const QString& nid, const QString& artUrl) {
    workWithPlayer([&] (MprisPlayer& p) {
      QVariantMap metadata;
      if (lengthUseconds >= 0) {
        metadata[Mpris::metadataToString(Mpris::Length)] = QVariant(lengthUseconds);
      }
      if (!title.isEmpty()) {
        metadata[Mpris::metadataToString(Mpris::Title)] = QVariant(title);
      }
      if (!nid.isEmpty()) {
        metadata[Mpris::metadataToString(Mpris::TrackId)] = QVariant("/com/Amazon/title/" + nid);
        //QString artUrl = getArtUrl(nid);
        if (!artUrl.isEmpty()) {
          metadata[Mpris::metadataToString(Mpris::ArtUrl)] = QVariant(artUrl);
        }
      }
      p.setMetadata(metadata);
    });
  });
}

void AmazonMprisInterface::volumeTimerFired() {
  getVolume([this] (double volume) {
    if (volume >= 0) {
      workWithPlayer([&] (MprisPlayer& p) {
        p.setVolume(volume);
      });
    }
  });
}

