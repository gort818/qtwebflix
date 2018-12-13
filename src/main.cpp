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

  //QCommandLineParser parser;

  MainWindow w;

  w.show();
  w.parseCommand();

  return app.exec();
}
