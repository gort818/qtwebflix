#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWebEngineView>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    webview = new QWebEngineView;

    ui->horizontalLayout->addWidget(webview);
    webview->setUrl(QUrl(QStringLiteral("http://netflix.com")));
    keyF11 = new QShortcut(this);   // Initialize the object
    keyF11->setKey(Qt::Key_F11);    // Set the key code
    // connect handler to keypress
    connect(keyF11, SIGNAL(activated()), this, SLOT(slotShortcutF11()));

    keyCtrlQ = new QShortcut(this); // Initialize the object
    keyCtrlQ->setKey(Qt::CTRL + Qt::Key_Q); // Set the key code
    // connect handler to keypress
    connect(keyCtrlQ, SIGNAL(activated()), this, SLOT(slotShortcutCtrlQ()));
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

/* Slot handler for Ctrl + D
 * */
void MainWindow::slotShortcutCtrlQ()
{
    /* For the treatment of a combination of keys Ctrl + D
     * will do the Message Box ,
     * which will signal that has been pressed a hotkey
     * */
    QApplication::quit();
}

