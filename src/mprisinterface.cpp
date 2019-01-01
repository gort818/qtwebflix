#include <QDebug>
#include <QWebEngineView>
#include <QWidget>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "mainwindow.h"
#include "mprisinterface.h"

MprisInterface::MprisInterface(QWidget *parent)
    : QObject(parent) {
}

void MprisInterface::setup(MainWindow *window) {
  this->window = window;
  webview = window->webView();

  std::lock_guard<std::mutex> l(mtx_player);
  player.setServiceName(QString("QtWebFlix"));
  player.setCanQuit(true);
  player.setCanSetFullscreen(true);
  player.setCanPause(true);
  player.setCanPlay(true);
  player.setCanControl(true);

  player.setMetadata(QVariantMap());

  prevTitleId = "";
  prevArtUrl = "";
  titleInfoFetching = false;

  connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkManagerFinished(QNetworkReply*)));

  connect(&playerStateTimer, SIGNAL(timeout()), this, SLOT(playerStateTimerFired()));
  playerStateTimer.start(500);

  connect(&playerPositionTimer, SIGNAL(timeout()), this, SLOT(playerPositionTimerFired()));
  playerPositionTimer.start(170);

  connect(&metadataTimer, SIGNAL(timeout()), this, SLOT(metadataTimerFired()));
  metadataTimer.start(1000);

  connect(&volumeTimer, SIGNAL(timeout()), this, SLOT(volumeTimerFired()));
  volumeTimer.start(220);

  connect(&player, SIGNAL(pauseRequested()), this, SLOT(pauseVideo()));
  connect(&player, SIGNAL(playRequested()), this, SLOT(playVideo()));
  connect(&player, SIGNAL(playPauseRequested()), this, SLOT(togglePlayPause()));
  connect(&player, SIGNAL(fullscreenRequested(bool)), this, SLOT(setFullScreen(bool)));
  connect(&player, SIGNAL(volumeRequested(double)), this, SLOT(setVideoVolume(double)));
}

void MprisInterface::playVideo() {
  QString code = ("(function () {" \
          "var vid = document.querySelector('video');" \
          "if (!vid) return;" \
          "vid.play();" \
          "})();");
  qDebug() << "Player playing";
  webview->page()->runJavaScript(code);
}

void MprisInterface::pauseVideo() {
  QString code = ("(function () {" \
          "var vid = document.querySelector('video');" \
          "if (!vid) return;" \
          "vid.pause();" \
          "})();");
  qDebug() << "Player paused";
  webview->page()->runJavaScript(code);
}

void MprisInterface::togglePlayPause() {
  QString code = ("(function () {" \
          "var vid = document.querySelector('video');" \
          "if (!vid) return;" \
          "if (vid.paused) vid.play();" \
          "else vid.pause();" \
          "})();");
  qDebug() << "Player toggled play/pause";
  webview->page()->runJavaScript(code);
}

void MprisInterface::setVideoVolume(double volume) {
  QString code = ("(function () {" \
          "var vid = document.querySelector('video');" \
          "if (!vid) return;" \
          "vid.volume = " + QString::number(volume) + ";" \
          "})();");
  qDebug() << "Player set volume to " << volume;
  webview->page()->runJavaScript(code);
}

void MprisInterface::setFullScreen(bool fullscreen) {
  window->setFullScreen(fullscreen);
}

void MprisInterface::getVolume(std::function<void(double)> callback) {
  QString code = ("(function () {" \
                 "var vid = document.querySelector('video');" \
                 "return vid ? vid.volume : -1;" \
          "})()");
  webview->page()->runJavaScript(code, [callback](const QVariant& result) {
            callback(result.toDouble());
          });
}

void MprisInterface::getVideoPosition(std::function<void(qlonglong)> callback) {
  QString code = ("(function () {" \
                 "var vid = document.querySelector('video');" \
                 "return vid ? vid.currentTime : -1;" \
          "})()");
  webview->page()->runJavaScript(code, [callback](const QVariant& result) {
            double seconds = result.toDouble();
            if (seconds < 0) callback(-1);
            else callback(seconds / 1e-6);
          });
}

void MprisInterface::getMetadata(std::function<void(qlonglong, const QString&, const QString&)> callback) {
  QString code = ("(function () {" \
                 "var vid = document.querySelector('video');" \
                 "var titleLabel = document.querySelector('.PlayerControls--control-element.video-title .ellipsize-text');" \
                 "var metadata = {};"\
                 "metadata.duration = vid ? vid.duration : -1;" \
                 "metadata.title = titleLabel ? titleLabel.innerHTML.replace(/(<([^>]+)>)/g, \" \").replace(/ +(?= )/g,'').replace(/ +(?= )/g,'').trim() : '';" \
                 "metadata.nid = vid && vid.offsetParent ? vid.offsetParent.id : '';" \
                 "return metadata;"
          "})()");
  webview->page()->runJavaScript(code, [callback](const QVariant& result) {
            QVariantMap map = result.toMap();

            double seconds = map["duration"].toDouble();
            if (seconds < 0) seconds = -1;
            else seconds /= 1e-6;

            QString title = map["title"].toString();
            QString nid = map["nid"].toString();

            callback(seconds, title, nid);
          });
}

void MprisInterface::getVideoState(std::function<void(Mpris::PlaybackStatus)> callback) {
  QString code = ("(function () {" \
                 "var vid = document.querySelector('video');" \
                 "if (!vid) return 'stopped';" \
                 "return vid.paused ? 'paused' : 'playing';" \
          "})()");
  webview->page()->runJavaScript(code, [callback](const QVariant& result) {
            QString resultString = result.toString();
            Mpris::PlaybackStatus status = Mpris::InvalidPlaybackStatus;
            if (resultString == "stopped") status = Mpris::Stopped;
            else if (resultString == "playing") status = Mpris::Playing;
            else if (resultString == "paused") status = Mpris::Paused;
            callback(status);
          });
}

void MprisInterface::playerStateTimerFired() {
    getVideoState([this](Mpris::PlaybackStatus state) {
            std::lock_guard<std::mutex> l(mtx_player);
            player.setPlaybackStatus(state);
          });
}

void MprisInterface::playerPositionTimerFired() {
    getVideoPosition([this](qlonglong useconds) {
            std::lock_guard<std::mutex> l(mtx_player);
            player.setPosition(useconds);
          });
}

void MprisInterface::metadataTimerFired() {
    getMetadata([this](qlonglong lengthUseconds, const QString& title, const QString& nid) {
            std::lock_guard<std::mutex> l(mtx_player);
            QVariantMap metadata;
            if (lengthUseconds >= 0) {
              metadata[Mpris::metadataToString(Mpris::Length)] = QVariant(lengthUseconds);
            }
            if (!title.isEmpty()) {
              metadata[Mpris::metadataToString(Mpris::Title)] = QVariant(title);
            }
            if (!nid.isEmpty()) {
              metadata[Mpris::metadataToString(Mpris::TrackId)] = QVariant("/com/netflix/title/" + nid);
              QString artUrl = getArtUrl(nid);
              if (!artUrl.isEmpty()) {
                metadata[Mpris::metadataToString(Mpris::ArtUrl)] = QVariant(artUrl);
              }
            }
            player.setMetadata(metadata);
          });
}

void MprisInterface::volumeTimerFired() {
    getVolume([this](double volume) {
            if (volume >= 0) {
              std::lock_guard<std::mutex> l(mtx_player);
              player.setVolume(volume);
            }
          });
}

void MprisInterface::updatePlayerFullScreen() {
  std::lock_guard<std::mutex> l(mtx_player);
  player.setFullscreen(window->isFullScreen());
}

QString MprisInterface::getArtUrl(const QString& nid) {
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
  titleInfoRequest.setAttribute(QNetworkRequest::FollowRedirectsAttribute, QVariant(true));

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

void MprisInterface::networkManagerFinished(QNetworkReply *reply) {
  if (!reply->error()) {
    QString html = reply->readAll();
    QRegExp rx("\"image\": *\"([^\"]*)\"");
    if (rx.indexIn(html) != -1) {
      prevArtUrl = rx.cap(1);
    } else {
      qDebug() << "Could not find art URL in title info response. Check the regex.";
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

