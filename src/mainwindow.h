#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <mutex>
#include <functional>

#include "urlrequestinterceptor.h"
#include <QAction>
#include <QByteArray>
#include <QCommandLineParser>
#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>
#include <QShortcut>
#include <QWebEngineFullScreenRequest>
#include <QWebEngineView>
#include <QTimer>
#include <Mpris>
#include <MprisPlayer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
//  void set_provider(QString);
//  void set_useragent(QString);
//  void parseCommand(QCommandLineParser &);
    void parseCommand();
  ~MainWindow();
  // QAction amazon();
  void getVideoState(std::function<void(Mpris::PlaybackStatus)> callback);
  void getVolume(std::function<void(double)> callback);

private slots:
  // slots for handlers of hotkeys
  void finishLoading(bool);
  void slotShortcutF11();
  void slotShortcutCtrlQ();
  void slotShortcutCtrlW();
  void slotShortcutCtrlS();
  void slotShortcutCtrlR();
  void slotShortcutCtrlF5();
  void ShowContextMenu(const QPoint &pos);

  void playVideo();
  void pauseVideo();
  void togglePlayPause();
  void setVideoVolume(double volume);
  void setFullScreen(bool fullscreen);

  void playerStateTimerFired();
  void volumeTimerFired();

protected:
  // save window geometry
  void closeEvent(QCloseEvent *);

private:
  Ui::MainWindow *ui;
  QWebEngineView *webview;
  QString jQuery;
  double playRate;
  QString playRateStr;
  QShortcut *keyF11;   // Entity of F11 hotkey
  QShortcut *keyCtrlQ; // Entity of Ctrl + D hotkeys
  QShortcut *keyCtrlW; // Entity of Crtl + W hotkey
  QShortcut *keyCtrlS; // Entity of Crtl + S hotkey
  QShortcut *keyCtrlR; // Entity of Crtl + R hotkey
  QShortcut *keyCtrlF5; // Entity of Crtl + R hotkey
  QSettings *appSettings;
  QSettings *provSettings;
  std::mutex mtx_player;
  MprisPlayer player;
  QTimer playerStateTimer;
  QTimer volumeTimer;
  void fullScreenRequested(QWebEngineFullScreenRequest request);
  void writeSettings();
  void readSettings();
  void restore();
  void updatePlayerFullScreen();

  UrlRequestInterceptor *m_interceptor;
};

#endif // MAINWINDOW_H
