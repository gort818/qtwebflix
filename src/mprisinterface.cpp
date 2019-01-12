#include <QWidget>

#include "mainwindow.h"
#include "mprisinterface.h"

MprisInterface::MprisInterface(QWidget *parent)
    : QObject(parent) {
}

void MprisInterface::setup(MainWindow *window) {
  m_window = window;

 //testing setting service name in the seperate interfaces
  workWithPlayer([] (MprisPlayer& p) {
    p.setServiceName("QtWebFlix");
  });
}


void MprisInterface::workWithPlayer(std::function<void(MprisPlayer&)> callback) {
  std::lock_guard<std::mutex> l(m_mtx_player);

  // This should be the ONLY point where `m_player` is accessed.
  // For anything else, use `workWithPlayer()`.
  callback(m_player);
}

MainWindow * MprisInterface::window() const {
  return m_window;
}

QWebEngineView * MprisInterface::webView() const {
  return m_window->webView();
}

void MprisInterface::updatePlayerFullScreen() {
  workWithPlayer([this] (MprisPlayer& p) {
    p.setFullscreen(m_window->isFullScreen());
  });
}

