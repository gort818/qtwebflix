#ifndef URLREQUESTINTERCEPTOR_H
#define URLREQUESTINTERCEPTOR_H


#include <QWebEngineUrlRequestInterceptor>

class UrlRequestInterceptor : public QWebEngineUrlRequestInterceptor
{
    Q_OBJECT

public:
    UrlRequestInterceptor(QObject *parent = nullptr);
    void interceptRequest(QWebEngineUrlRequestInfo &info) override;
};
#endif // URLREQUESTINTERCEPTOR_H
