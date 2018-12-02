#include "commandlineparser.h"
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

  QCoreApplication::setApplicationName("qtwebflix");
  QCoreApplication::setApplicationVersion("n/a");

  QCommandLineParser parser;
  parser.setApplicationDescription("Qtwebflix Help");
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption setProvider(
      QStringList() << "p"
                    << "provider",
      QCoreApplication::translate("main",
                                  "Set content provider eg. netflix.com"),
      QCoreApplication::translate("main", "provider"));
  parser.addOption(setProvider);

  QString widevine = "--register-pepper-plugins";

  QStringList args;

  args = qApp->arguments();
  // qDebug()<<args;
  args.replaceInStrings("--register-pepper-plugins", "");

  parser.process(args);
  // const QStringList args = parser.positionalArguments();

  QString provider = parser.value(setProvider);

  MainWindow w;

  w.show();
  if (parser.isSet(setProvider)) {
    qDebug() << "Provider is set";
    w.set_provider(provider);
  }
  return app.exec();
}
