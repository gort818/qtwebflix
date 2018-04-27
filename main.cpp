#include "mainwindow.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QProcess>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineUrlRequestInterceptor>
#include <QWebEngineView>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);


  MainWindow w;

  w.show();
  return app.exec();
}
