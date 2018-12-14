#include "commandlineparser.h"

Commandlineparser::Commandlineparser() {

  // create commandline parser, set options, and parse them.
  QCommandLineParser parser;
  QCoreApplication::setApplicationName("qtwebflix");
  QCoreApplication::setApplicationVersion(QVariant(GIT_VERSION).toString());
  parser.setApplicationDescription(
      "Qtwebflix Help \n\n To Control playback rate :\n CTRL + W = speed up \n "
      "CTRL + S = slow down \n CTRL + R = reset to defualt\n");
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption setProvider(
      QStringList() << "p"
                    << "provider",
      QCoreApplication::translate("main",
                                  "Set content provider eg. netflix.com"),
      QCoreApplication::translate("main", "provider"));
  parser.addOption(setProvider);

  QCommandLineOption userAgent(
      QStringList() << "u"
                    << "useragent",
      QCoreApplication::translate(
          "main", "change useragent eg. \"Mozilla/5.0 (X11; Linux x86_64; "
                  "rv:63.0) Gecko/20100101 Firefox/63.0\""),
      QCoreApplication::translate("main", "useragent"));
  parser.addOption(userAgent);

  QStringList webOptions = {"--register-pepper-plugins",
                            "--disable-seccomp-filter-sandbox",
                            "--disable-logging"};

  QStringList args;

  args = qApp->arguments();
  // qDebug()<<args;
  for (auto arg : args) {
    for (auto webOption : webOptions) {
      if (arg.startsWith(webOption)) {
        args.replaceInStrings(webOption, "");
      }
    }
  }
  parser.process(args);

  if (parser.isSet(setProvider)) {
    qDebug() << "Provider is set";
    providerSet_ = true;
    provider_ = parser.value(setProvider);

  } else {
    providerSet_ = false;
  }

  if (parser.isSet(userAgent)) {
    qDebug() << "useragent is set";
    userAgentset_ = true;
    userAgent_ = parser.value(userAgent);

  } else {
    userAgentset_ = false;
  }
}

bool Commandlineparser::providerIsSet() const { return providerSet_; }

bool Commandlineparser::userAgentisSet() const { return userAgentset_; }

QString Commandlineparser::getProvider() const { return provider_; }

QString Commandlineparser::getUserAgent() const { return userAgent_; }
