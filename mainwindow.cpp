#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QWebEngineFullScreenRequest>
#include <QSettings>
#include <QWidget>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    appSettings = new QSettings("Qtwebflix","Save State", this);
    ui->setupUi(this);
    readSettings();
    webview = new QWebEngineView;
    ui->horizontalLayout->addWidget(webview);
    webview->setUrl(QUrl(QStringLiteral("https://netflix.com")));
    webview->settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
    //connect handler for fullscreen press on video
    connect(webview->page(),
                &QWebEnginePage::fullScreenRequested,
                this,
                &MainWindow::fullScreenRequested);

    keyF11 = new QShortcut(this);   // Initialize the object
    keyF11->setKey(Qt::Key_F11);    // Set the key code
    // connect handler to keypress
    connect(keyF11, SIGNAL(activated()), this, SLOT(slotShortcutF11()));

    keyCtrlQ = new QShortcut(this); // Initialize the object
    keyCtrlQ->setKey(Qt::CTRL + Qt::Key_Q); // Set the key code
    // connect handler to keypress
    connect(keyCtrlQ, SIGNAL(activated()), this, SLOT(slotShortcutCtrlQ()));
    QSettings settings;
    restoreState(settings.value("mainWindowState").toByteArray());
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Slot handler of F11
 * */
void MainWindow::slotShortcutF11()
{
    /* This handler will make switching applications in full screen mode
     * and back to normal window mode
     * */
    if(this->isFullScreen()){
        this->showNormal();
    } else {
        this->showFullScreen();
    }
}

/* Slot handler for Ctrl + Q
 * */
void MainWindow::slotShortcutCtrlQ()
{
    /* For the treatment of a combination of keys Ctrl + Q
     * will do the Message Box ,
     * which will signal that has been pressed a hotkey
     * */
    writeSettings();
    QApplication::quit();
}

void MainWindow::closeEvent(QCloseEvent *)
{
    // This will be called whenever this window is closed.
    writeSettings();
}

void MainWindow::writeSettings()
{
    // Write the values to disk in categories.
    appSettings->setValue("state/mainWindowState", saveState());
    appSettings->setValue("geometry/mainWindowGeometry",
                                           saveGeometry());
}

void MainWindow::restore()
{
    // We have to call toByteArray because the
    // values are stored as QVariant's
    QByteArray stateData = appSettings->value
            ("state/mainWindowState").toByteArray();

    QByteArray geometryData = appSettings->value
            ("geometry/mainWindowGeometry").toByteArray();

    restoreState(stateData);
    restoreGeometry(geometryData);
}

void MainWindow::readSettings()
{
    // This is a call to our restore() method.
    // We use this so we can keep the purposes of the
    // different methods clear if we were to expand.
    restore();
}

void MainWindow::fullScreenRequested(QWebEngineFullScreenRequest request)
{
    //fullscreen on video players
    if(request.toggleOn())
    {
        request.accept();
        this->showFullScreen();

     }
    else
    {
        request.accept();
        this->showNormal();
    }

}
