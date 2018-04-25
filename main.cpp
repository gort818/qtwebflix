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

  QCommandLineParser parser;
  parser.setApplicationDescription(
      "\nQtwebFlix Help\n\n If you are having trouble accessing DRM content "
      "please add this commandline option:\n"
      "--register-pepper-plugins=\"/usr/lib/qt/plugins/ppapi/"
      "libwidevinecdmadapter.so; application/x-ppapi-widevine-cdm\"");
  parser.addHelpOption();

  parser.process(QApplication::arguments());

  MainWindow w;

  w.show();
  return app.exec();
}
