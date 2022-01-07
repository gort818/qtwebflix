#include <QApplication>
#include <QLibraryInfo>
#include <QProcess>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineUrlRequestInterceptor>
#include <QWebEngineView>

#include "mainwindow.h"

//#include <KAboutData>

int main(int argc, char *argv[]) {
  qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-features=MediaSessionService");
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
  QApplication app(argc, argv);
  QApplication::setWindowIcon(QIcon(":/resources/qtwebflix.svg"));


  MainWindow w;

  w.show();
  w.parseCommand();

  return app.exec();
}
