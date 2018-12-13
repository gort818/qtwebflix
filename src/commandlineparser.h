#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H
#include "mainwindow.h"

class Commandlineparser {
public:
  Commandlineparser();

  QString getProvider() const;
  QString getUserAgent() const;
  bool providerIsSet() const;
  bool userAgentisSet() const;

private:
  QString provider_;
  QString userAgent_;
  bool providerSet_;
  bool userAgentset_;
};

#endif // COMMANDLINEPARSER_H
