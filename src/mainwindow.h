#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <functional>
#include <memory>
#include <typeindex>

#include <QAction>
#include <QByteArray>
#include <QCommandLineParser>
#include <QMainWindow>
#include <QMap>
#include <QMenu>
#include <QMessageBox>
#include <QPair>
#include <QSet>
#include <QSettings>
#include <QShortcut>
#include <QWebEngineFullScreenRequest>
#include <QWebEngineView>

#include "mprisinterface.h"
#include "urlrequestinterceptor.h"

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
  void setFullScreen(bool fullscreen);
  QWebEngineView *webView() const;

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

protected:
  // save window geometry
  void closeEvent(QCloseEvent *);

private:
  Ui::MainWindow *ui;
  QWebEngineView *webview;
  QString jQuery;

  double playRate;
  QString playRateStr;

  QSettings *stateSettings;
  QSettings *appSettings;

  QMenu contextMenu;

  std::type_index mprisType;
  std::unique_ptr<MprisInterface> mpris;

  void fullScreenRequested(QWebEngineFullScreenRequest request);
  void writeSettings();
  void readSettings();
  void restore();
  void exchangeMprisInterfaceIfNeeded();
  void addShortcut(const QString &, const QString &);
  void registerMprisShortcutActions();
  void createContextMenu(const QStringList &keys);

  QMap<QString, std::pair<const QObject *, const char *>> m_actions;
  std::map<QString, QSet<const QShortcut *>> m_shortcuts;

  UrlRequestInterceptor *m_interceptor;

  template <typename Interface> bool setMprisInterface() {
    std::type_index newType(typeid(Interface));
    if (mprisType == newType) {
      return false;
    }

    qDebug() << "Transitioning to new MPRIS interface: "
             << typeid(Interface).name();
    mprisType = newType;
    mpris.reset();

    mpris = std::make_unique<Interface>();
    mpris->setup(this);
    registerMprisShortcutActions();

    return true;
  }
};

#endif // MAINWINDOW_H
