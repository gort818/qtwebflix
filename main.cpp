#include <QWebEngineView>
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(QLatin1String("netflix")));
    MainWindow w;
   
    w.setWindowState(Qt::WindowMaximized);
    w.show();
    return app.exec();
}
