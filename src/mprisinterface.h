#ifndef MPRISINTERFACE_H
#define MPRISINTERFACE_H

#include <mutex>
#include <functional>

#include <QTimer>
#include <QWebEngineView>
#include <Mpris>
#include <MprisPlayer>

class MainWindow;

class MprisInterface : public QObject {
  Q_OBJECT

public:
  explicit MprisInterface(QWidget *parent = nullptr);

  void setup(MainWindow *window);

  void getVideoState(std::function<void(Mpris::PlaybackStatus)> callback);
  void getVideoPosition(std::function<void(qlonglong)> callback);
  void getMetadata(std::function<void(qlonglong, const QString&, const QString&)> callback);
  void getVolume(std::function<void(double)> callback);

  void updatePlayerFullScreen();

private slots:
  // slots for handlers of hotkeys
  void playVideo();
  void pauseVideo();
  void togglePlayPause();
  void setVideoVolume(double volume);
  void setFullScreen(bool fullscreen);

  void playerStateTimerFired();
  void playerPositionTimerFired();
  void metadataTimerFired();
  void volumeTimerFired();

  void networkManagerFinished(QNetworkReply *reply);

private:
  QWebEngineView *webview;
  MainWindow *window;
  std::mutex mtx_player;
  MprisPlayer player;
  QTimer playerStateTimer;
  QTimer playerPositionTimer;
  QTimer metadataTimer;
  QTimer volumeTimer;
  QNetworkAccessManager networkManager;
  QString prevTitleId;
  QString prevArtUrl;
  std::mutex mtx_titleInfo;
  bool titleInfoFetching;
  QString getArtUrl(const QString& nid);


};

#endif // MPRISINTERFACE_H
