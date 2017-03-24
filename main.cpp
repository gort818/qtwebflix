#include <QWebEngineView>
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(QLatin1String("data/netflix.svg")));
    MainWindow w;
   
    w.show();
    return app.exec();
}
