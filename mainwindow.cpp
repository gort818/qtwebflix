#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "urlrequestinterceptor.h"
#include <QContextMenuEvent>
#include <QDebug>
#include <QMenu>
#include <QSettings>
#include <QStandardPaths>
#include <QWebEngineFullScreenRequest>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineUrlRequestInterceptor>
#include <QWebEngineView>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)

{
  QWebEngineSettings::globalSettings()->setAttribute(
      QWebEngineSettings::PluginsEnabled, true);
  appSettings = new QSettings("Qtwebflix", "Save State", this);

  ui->setupUi(this);
  this->setWindowTitle("QtWebFlix");
  readSettings();
  webview = new QWebEngineView;
  ui->horizontalLayout->addWidget(webview);

  if (appSettings->value("site").toString() == "") {
    webview->setUrl(QUrl(QStringLiteral("http://netflix.com")));

  } else {
    webview->setUrl(QUrl(appSettings->value("site").toString()));
  }
  webview->settings()->setAttribute(
      QWebEngineSettings::FullScreenSupportEnabled, true);

  // connect handler for fullscreen press on video
  connect(webview->page(), &QWebEnginePage::fullScreenRequested, this,
          &MainWindow::fullScreenRequested);

  this->m_interceptor = new UrlRequestInterceptor;
  this->webview->page()->profile()->setRequestInterceptor(this->m_interceptor);

  // Check if user is using arm processor(Raspberry pi)
  QString UserAgent = this->webview->page()->profile()->httpUserAgent();
  qDebug() << UserAgent;
  qDebug() << UserAgent.contains("Linux arm");
  if (UserAgent.contains("Linux arm")) {
    qDebug() << "Changing user agent for raspberry pi users";
    QString UserAgent = "Mozilla/5.0 (X11; CrOS armv7l 6946.86.0) "
                        "AppleWebKit/537.36 (KHTML, like Gecko) "
                        "Chrome/51.0.2704.91 Safari/537.36";
    this->webview->page()->profile()->setHttpUserAgent(UserAgent);
  }

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
  QString site = webview->url().toString();
  appSettings->setValue("site", site);
  qDebug() << " write settings:" << site;
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
  provSettings = new QSettings("Qtwebflix", "Providers", this);
  provSettings->setIniCodec("UTF-8");
  provSettings->beginGroup("providers");
  provSettings->setValue("netflix", "http://netflix.com");
  QString test = provSettings->value("netflix").toString();
  provSettings->sync();
  QStringList keys = provSettings->allKeys();

  QMenu myMenu;
  for (const auto &i : keys) {
    qDebug() << "keys" << i;
    if (!i.startsWith("#")) {
      myMenu.addAction(i);
      myMenu.addSeparator();
    } else {
      QFile conf(provSettings->fileName());
      conf.open(QIODevice::ReadWrite | QIODevice::Text);
      QByteArray fileContent = conf.readAll();
      fileContent.replace(QString("\%23"), "#");
      conf.seek(0);
      conf.write(fileContent);
      conf.close();
    }
  }

  QAction *selectedItem = myMenu.exec(globalPos);

  if (selectedItem == NULL) {
    return;
  } else if (selectedItem) {
    QString url = provSettings->value(selectedItem->text()).toString();
    qDebug() << "URL is :" << url;
    webview->setUrl(QUrl(url));
    provSettings->endGroup();
  }

  else {
    // nothing was chosen
  }
}
