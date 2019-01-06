#ifndef MPRISINTERFACE_H
#define MPRISINTERFACE_H

#include <mutex>
#include <functional>

#include <QWebEngineView>
#include <Mpris>
#include <MprisPlayer>

class MainWindow;

class MprisInterface : public QObject {
  Q_OBJECT

public:
  explicit MprisInterface(QWidget *parent = nullptr);
  virtual ~MprisInterface() = default;

  virtual void setup(MainWindow *window);

  void updatePlayerFullScreen();

protected:
  void workWithPlayer(std::function<void(MprisPlayer&)> callback);
  MainWindow * window() const;
  QWebEngineView * webView() const;

private:
  MainWindow *m_window;
  std::mutex m_mtx_player;
  MprisPlayer m_player;

};

#endif // MPRISINTERFACE_H
