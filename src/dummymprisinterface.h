#ifndef DUMMYMPRISINTERFACE_H
#define DUMMYMPRISINTERFACE_H

#include "mprisinterface.h"

class MainWindow;

class DummyMprisInterface : public MprisInterface {
  Q_OBJECT

public:
  explicit DummyMprisInterface(QWidget *parent = nullptr);

};

#endif // DUMMYMPRISINTERFACE_H
