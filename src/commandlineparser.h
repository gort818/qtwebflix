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
  bool nonHDisSet() const;
private:
  QString provider_;
  QString userAgent_;
  bool providerSet_;
  bool userAgentset_;
  bool nonHDset_;
};

#endif // COMMANDLINEPARSER_H
