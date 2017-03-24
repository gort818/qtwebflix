#include <QWebEngineView>
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(QLatin1String("/data/netflix.svg")));// Set Location of Icon
    MainWindow w;// Create the window
    w.show();//Show the window
    return app.exec();
}
