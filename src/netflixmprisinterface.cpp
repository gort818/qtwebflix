#include "netflixmprisinterface.h"
#include "mainwindow.h"
#include "mprisinterface.h"
#include <QDebug>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QWebEngineProfile>
#include <QWebEngineView>
#include <QWidget>

NetflixMprisInterface::NetflixMprisInterface(QWidget *parent)
    : MprisInterface(parent) {
  prevTitleId = "";
  prevArtUrl = "";
  titleInfoFetching = false;
}

void NetflixMprisInterface::setup(MainWindow *window) {
  MprisInterface::setup(window);

  workWithPlayer([this](MprisPlayer &p) {
    // Expose player capabilities.
    p.setCanQuit(true);
    p.setCanSetFullscreen(true);
    p.setCanPause(true);
    p.setCanPlay(true);
    p.setCanControl(true);
    p.setCanSeek(true);
    p.canSeek();
    p.canSeekChanged();
    p.setMetadata(QVariantMap());
    p.setIdentity("Qtwebflix -- Netflix");

    connect(&p, SIGNAL(pauseRequested()), this, SLOT(pauseVideo()));
    connect(&p, SIGNAL(playRequested()), this, SLOT(playVideo()));
    connect(&p, SIGNAL(playPauseRequested()), this, SLOT(togglePlayPause()));
    connect(&p, SIGNAL(nextRequested()), this, SLOT(goNextEpisode()));
    connect(&p, SIGNAL(fullscreenRequested(bool)), this,
            SLOT(setFullScreen(bool)));
    connect(&p, SIGNAL(volumeRequested(double)), this,
            SLOT(setVideoVolume(double)));
    connect(&p, SIGNAL(setPositionRequested(QDBusObjectPath, qlonglong)), this,
            SLOT(setPosition(QDBusObjectPath, qlonglong)));
    connect(&p, SIGNAL(seekRequested(qlonglong)), this,
            SLOT(setSeek(qlonglong)));
  });

  connect(&networkManager, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(networkManagerFinished(QNetworkReply *)));

  // Connect slots and start timers.
  connect(&playerStateTimer, SIGNAL(timeout()), this,
          SLOT(playerStateTimerFired()));
  playerStateTimer.start(500);

  connect(&playerPositionTimer, SIGNAL(timeout()), this,
          SLOT(playerPositionTimerFired()));
  playerPositionTimer.start(170);

  connect(&metadataTimer, SIGNAL(timeout()), this, SLOT(metadataTimerFired()));
  metadataTimer.start(200);

  connect(&volumeTimer, SIGNAL(timeout()), this, SLOT(volumeTimerFired()));
  volumeTimer.start(220);

  connect(&goNextTimer, SIGNAL(timeout()), this, SLOT(goNextTimerFired()));
  goNextTimer.start(5000);
}

void NetflixMprisInterface::playVideo() {
  QString code = ("(function () {"
                  "var vid = document.querySelector('video');"
                  "if (!vid) return;"
                  "vid.play();"
                  "})();");
  qDebug() << "Player playing";
  webView()->page()->runJavaScript(code);
}

void NetflixMprisInterface::pauseVideo() {
  QString code = ("(function () {"
                  "var vid = document.querySelector('video');"
                  "if (!vid) return;"
                  "vid.pause();"
                  "})();");
  qDebug() << "Player paused";
  webView()->page()->runJavaScript(code);
}

void NetflixMprisInterface::togglePlayPause() {
  QString code = ("(function () {"
                  "var vid = document.querySelector('video');"
                  "if (!vid) return;"
                  "if (vid.paused) vid.play();"
                  "else vid.pause();"
                  "})();");
  qDebug() << "Player toggled play/pause";
  webView()->page()->runJavaScript(code);
}

void NetflixMprisInterface::goNextEpisode() {

  QString code = ("(function () {"
                  "var goNext = "
                  "document.querySelector('button.touchable.PlayerControls--"
                  "control-element.nfp-button-control.default-control-button."
                  "button-nfplayerNextEpisode');"
                  "goNext.click();"
                  "})()");
  qDebug() << "Next episode";
  webView()->page()->runJavaScript(code);
}

void NetflixMprisInterface::setVideoVolume(double volume) {
  QString code = ("(function () {"
                  "var vid = document.querySelector('video');"
                  "if (!vid) return;"
                  "vid.volume = " +
                  QString::number(volume) +
                  ";"
                  "})();");
  qDebug() << "Player set volume to " << volume;
  webView()->page()->runJavaScript(code);
}

void NetflixMprisInterface::setFullScreen(bool fullscreen) {
  window()->setFullScreen(fullscreen);
}

void NetflixMprisInterface::getVolume(std::function<void(double)> callback) {
  QString code = ("(function () {"
                  "var vid = document.querySelector('video');"
                  "return vid ? vid.volume : -1;"
                  "})()");
  webView()->page()->runJavaScript(code, [callback](const QVariant &result) {
    callback(result.toDouble());
  });
}

void NetflixMprisInterface::getVideoPosition(
    std::function<void(qlonglong)> callback) {
  QString code = ("(function () {"
                  "var vid = document.querySelector('video');"
                  "return vid ? vid.currentTime : -1;"
                  "})()");
  webView()->page()->runJavaScript(code, [callback](const QVariant &result) {
    double seconds = result.toDouble();
    if (seconds < 0)
      callback(-1);
    else
      callback(seconds / 1e-6);
  });
}

void NetflixMprisInterface::setPosition(QDBusObjectPath trackId,
                                        qlonglong pos) {
  double useconds = static_cast<double>(pos);
  useconds = useconds / 1000;
  qDebug() << "Seeking Position by " << useconds / 1000 << " Seconds";
  QString code = ("(function () {"
                  "const videoPlayer = netflix"
                  ".appContext"
                  ".state"
                  ".playerApp"
                  ".getAPI()"
                  ".videoPlayer;"
                  "const playerSessionId = videoPlayer"
                  ".getAllPlayerSessionIds()[0];"
                  "const player = videoPlayer"
                  ".getVideoPlayerBySessionId(playerSessionId);"
                  "player.seek(" +
                  QString::number(useconds) +
                  ");"
                  "})();");
  webView()->page()->runJavaScript(code);
  qDebug("running");
}

void NetflixMprisInterface::setSeek(qlonglong seekPos) {
  double useconds = static_cast<double>(seekPos);
  useconds = useconds / 1000;
  qDebug() << "Seeking Position by " << useconds / 1000 << " Seconds";
  QString code = ("(function () {"
                  "const videoPlayer = netflix"
                  ".appContext"
                  ".state"
                  ".playerApp"
                  ".getAPI()"
                  ".videoPlayer;"
                  "const playerSessionId = videoPlayer"
                  ".getAllPlayerSessionIds()[0];"
                  "const player = videoPlayer"
                  ".getVideoPlayerBySessionId(playerSessionId);"
                  "player.seek(player.getCurrentTime() +" +
                  QString::number(useconds) +
                  ");"
                  "})();");
  webView()->page()->runJavaScript(code);
}
void NetflixMprisInterface::getMetadata(
    std::function<void(qlonglong, const QString &, const QString &)> callback) {
  QString code =
      ("(function () {"
       "var vid = document.querySelector('video');"
       "const titleLabel = "
       "document.querySelectorAll(\"[data-uia='video-title']\")[0];"
       "var metadata = {};"
       "metadata.duration = vid ? vid.duration : -1;"
       "metadata.title = titleLabel ? "
       "titleLabel.innerHTML.replace(/(<([^>]+)>)/g, \" \").replace(/ +(?= "
       ")/g,'').replace(/ +(?= )/g,'').trim() : '';"
       "metadata.nid = vid && vid.offsetParent ? vid.offsetParent.id : '';"
       "return metadata;"
       "})()");
  webView()->page()->runJavaScript(code, [callback](const QVariant &result) {
    QVariantMap map = result.toMap();

    double seconds = map["duration"].toDouble();
    if (seconds < 0)
      seconds = -1;
    else
      seconds /= 1e-6;

    QString nid = map["nid"].toString();
    if (map["title"].toString() != "") {
      QString title = map["title"].toString();
      map["title"] = title;
      callback(seconds, title, nid);
    }
  });
}

void NetflixMprisInterface::getVideoState(
    std::function<void(Mpris::PlaybackStatus)> callback) {
  QString code = ("(function () {"
                  "var vid = document.querySelector('video');"
                  "if (!vid) return 'stopped';"
                  "return vid.paused ? 'paused' : 'playing';"
                  "})()");
  webView()->page()->runJavaScript(code, [callback](const QVariant &result) {
    QString resultString = result.toString();
    Mpris::PlaybackStatus status = Mpris::InvalidPlaybackStatus;
    if (resultString == "stopped")
      status = Mpris::Stopped;
    else if (resultString == "playing")
      status = Mpris::Playing;
    else if (resultString == "paused")
      status = Mpris::Paused;
    callback(status);
  });
}

void NetflixMprisInterface::playerStateTimerFired() {
  getVideoState([this](Mpris::PlaybackStatus state) {
    workWithPlayer([&](MprisPlayer &p) { p.setPlaybackStatus(state); });
  });
}

void NetflixMprisInterface::playerPositionTimerFired() {
  getVideoPosition([this](qlonglong useconds) {
    workWithPlayer([&](MprisPlayer &p) { p.setPosition(useconds); });
  });
}

void NetflixMprisInterface::metadataTimerFired() {
  getMetadata([this](qlonglong lengthUseconds, const QString &title,
                     const QString &nid) {
    workWithPlayer([&](MprisPlayer &p) {
      QVariantMap metadata;
      if (lengthUseconds >= 0) {
        metadata[Mpris::metadataToString(Mpris::Length)] =
            QVariant(lengthUseconds);
      }
      if (!title.isEmpty()) {
        metadata[Mpris::metadataToString(Mpris::Title)] = QVariant(title);
      }
      if (!nid.isEmpty()) {
        metadata[Mpris::metadataToString(Mpris::TrackId)] =
            QVariant("/com/netflix/title/" + nid);
        QString artUrl = getArtUrl(nid);
        if (!artUrl.isEmpty()) {
          metadata[Mpris::metadataToString(Mpris::ArtUrl)] = QVariant(artUrl);
        }
      }
      p.setMetadata(metadata);
    });
  });
}

void NetflixMprisInterface::volumeTimerFired() {
  getVolume([this](double volume) {
    if (volume >= 0) {
      workWithPlayer([&](MprisPlayer &p) { p.setVolume(volume); });
    }
  });
}

QString NetflixMprisInterface::getArtUrl(const QString &nid) {
  std::lock_guard<std::mutex> l(mtx_titleInfo);

  if (nid.isEmpty()) {
    return QString();
  }

  if (nid == prevTitleId && !prevArtUrl.isEmpty()) {
    // The URL was asynchronously retrieved earlier.
    return prevArtUrl;
  }

  if (titleInfoFetching) {
    // GET request already in progress.
    return QString();
  }

  QUrl jsonUrl("https://www.netflix.com/title/" + nid);
  QNetworkRequest titleInfoRequest(jsonUrl);
  titleInfoRequest.setRawHeader("Accept", "application/json");
  titleInfoRequest.setAttribute(QNetworkRequest::FollowRedirectsAttribute,
                                QVariant(true));

  // Here we assume that nobody but `metadataTimerFired` calls us.
  // Since the timer ticks every second, it's unlikely that we will make
  // duplicate requests. Otherwise we would need locks.
  titleInfoFetching = true;
  prevArtUrl = QString();
  prevTitleId = nid;

  networkManager.get(titleInfoRequest);

  // The request's under way. Hopefully, next time around the response will have
  // arrived.
  return QString();
}

void NetflixMprisInterface::networkManagerFinished(QNetworkReply *reply) {
  if (!reply->error()) {
    QString html = reply->readAll();
    QRegExp rx("\"image\": *\"([^\"]*)\"");
    if (rx.indexIn(html) != -1) {
      prevArtUrl = rx.cap(1);
    } else {
      qDebug()
          << "Could not find art URL in title info response. Check the regex.";
    }
  } else {
    qDebug() << "Title info request failed with error:" << reply->errorString();
  }

  {
    std::lock_guard<std::mutex> l(mtx_titleInfo);
    titleInfoFetching = false;
  }

  reply->deleteLater();
}

void NetflixMprisInterface::goNextTimerFired() {
  QString code = ("(function () {"
                  "var goNext = "
                  "document.querySelector('button.touchable.PlayerControls--"
                  "control-element.nfp-button-control.default-control-button."
                  "button-nfplayerNextEpisode');"
                  "if (!goNext) return false;"
                  "else return true;"
                  "})()");
  webView()->page()->runJavaScript(code, [this](const QVariant &result) {
    QString resultString = result.toString();
    if (resultString == "false") {
      workWithPlayer([](MprisPlayer &p) { p.setCanGoNext(false); });
    } else {
      workWithPlayer([](MprisPlayer &p) { p.setCanGoNext(true); });
    }
  });
}
