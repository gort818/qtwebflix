#include "mainwindow.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QWebEngineSettings>
#include <QWebEngineView>
#include <iostream>
int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  //app.setWindowIcon(QIcon(QLatin1String("netflix-desktop")));
  MainWindow w;

  w.show();
  return app.exec();
}
