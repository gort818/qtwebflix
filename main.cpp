#include <QWebEngineView>
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(QLatin1String("/home/alessandro/netflix/data/netflix.svg")));
    MainWindow w;
   // QWebEngineView view;
    //view.setUrl(QUrl(QStringLiteral("http://netflix.com")));
    //view.showNormal();
    w.show();
    //view.destroy(1);
    //setCentralWidget(view);
    return app.exec();
}
