#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShortcut>
#include <QWebEngineView>
#include <QWebEngineFullScreenRequest>
#include <QMessageBox>
#include <QSettings>
#include <QByteArray>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    // slots for handlers of hotkeys
    void slotShortcutF11();
    void slotShortcutCtrlQ();


protected:
    // save window geometry
    void closeEvent(QCloseEvent *);



private:
    Ui::MainWindow  *ui;
    QWebEngineView* webview;
    QShortcut       *keyF11;    // Entity of F11 hotkey
    QShortcut       *keyCtrlQ;  // Entity of Ctrl + D hotkeys
    QSettings *appSettings;
    void fullScreenRequested(QWebEngineFullScreenRequest request);
    void writeSettings();
    void readSettings();
    void restore();
};

#endif // MAINWINDOW_H


