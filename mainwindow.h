#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShortcut>
#include <QWebEngineView>
#include <QMessageBox>

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

private:
    Ui::MainWindow  *ui;
    QWebEngineView* webview;
    QShortcut       *keyF11;    // Entity of F11 hotkey
    QShortcut       *keyCtrlQ;  // Entity of Ctrl + D hotkeys
};

#endif // MAINWINDOW_H


