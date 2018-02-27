#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QContextMenuEvent>
#include <QMenu>
#include <QSettings>
#include <QWebEngineFullScreenRequest>
#include <QWebEngineSettings>
#include <QWebEngineView>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)

{
  appSettings = new QSettings("Qtwebflix", "Save State", this);
  ui->setupUi(this);
  readSettings();
  webview = new QWebEngineView;
  ui->horizontalLayout->addWidget(webview);
  webview->setUrl(QUrl(QStringLiteral("https://netflix.com")));
  webview->settings()->setAttribute(
      QWebEngineSettings::FullScreenSupportEnabled, true);

  // connect handler for fullscreen press on video
  connect(webview->page(), &QWebEnginePage::fullScreenRequested, this,
          &MainWindow::fullScreenRequested);

  // key short cuts
  keyF11 = new QShortcut(this); // Initialize the object
  keyF11->setKey(Qt::Key_F11);  // Set the key code
  // connect handler to keypress
  connect(keyF11, SIGNAL(activated()), this, SLOT(slotShortcutF11()));
  keyCtrlQ = new QShortcut(this);         // Initialize the object
  keyCtrlQ->setKey(Qt::CTRL + Qt::Key_Q); // Set the key code
  // connect handler to keypress
  connect(keyCtrlQ, SIGNAL(activated()), this, SLOT(slotShortcutCtrlQ()));

  // Window size settings
  QSettings settings;
  restoreState(settings.value("mainWindowState").toByteArray());

  webview->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(webview, SIGNAL(customContextMenuRequested(const QPoint &)), this,
          SLOT(ShowContextMenu(const QPoint &)));
}

MainWindow::~MainWindow() { delete ui; }

// Slot handler of F11
void MainWindow::slotShortcutF11() {
  /* This handler will make switching applications in full screen mode
   * and back to normal window mode
   * */
  if (this->isFullScreen()) {
    this->showNormal();
  } else {
    this->showFullScreen();
  }
}

// Slot handler for Ctrl + Q
void MainWindow::slotShortcutCtrlQ() {
  writeSettings();
  QApplication::quit();
}

void MainWindow::closeEvent(QCloseEvent *) {
  // This will be called whenever this window is closed.
  writeSettings();
}

void MainWindow::writeSettings() {
  // Write the values to disk in categories.
  appSettings->setValue("state/mainWindowState", saveState());
  appSettings->setValue("geometry/mainWindowGeometry", saveGeometry());
}

void MainWindow::restore() {
  QByteArray stateData =
      appSettings->value("state/mainWindowState").toByteArray();

  QByteArray geometryData =
      appSettings->value("geometry/mainWindowGeometry").toByteArray();

  restoreState(stateData);
  restoreGeometry(geometryData);
}

void MainWindow::readSettings() { restore(); }

void MainWindow::fullScreenRequested(QWebEngineFullScreenRequest request) {

  // fullscreen on video players

  if (request.toggleOn()) {
    this->showFullScreen();
  } else {
    this->showNormal();
  }
  request.accept();
}

void MainWindow::ShowContextMenu(const QPoint &pos) // this is a slot
{
  QPoint globalPos = webview->mapToGlobal(pos);

  QMenu myMenu;
  myMenu.addAction("Amazon Prime");
  myMenu.addAction("Netflix");
  myMenu.addAction("Hulu");
  QAction *selectedItem = myMenu.exec(globalPos);
  if (selectedItem == NULL) {
    return;
  } else if (selectedItem->text() == "Amazon Prime") {
    webview->setUrl(QUrl(QStringLiteral(
        "https://www.amazon.com/Amazon-Video/b/?&node=2858778011")));
  }

  else if (selectedItem->text() == "Hulu") {
    webview->setUrl(QUrl(QStringLiteral("https://hulu.com")));
  }

  else if (selectedItem->text() == "Netflix") {
     webview->setUrl(QUrl(QStringLiteral("https://netflix.com")));
    myMenu.clear();
    myMenu.addAction("test");
  }

  else {
    // nothing was chosen
  }
}
