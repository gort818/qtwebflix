#include "mprisinterface.h"
#include "dummymprisinterface.h"

DummyMprisInterface::DummyMprisInterface(QWidget *parent)
    : MprisInterface(parent) {
  /* Nothing done. This object is truly a dummy. */
}

