#include "urlrequestinterceptor.h"

#include <QWebEngineUrlRequestInterceptor>

#include <QDebug>

UrlRequestInterceptor::UrlRequestInterceptor(QObject *parent)
    : QWebEngineUrlRequestInterceptor(parent)
{
}

void UrlRequestInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info)
{
    static const QRegExp netflix1080p_pattern(R"(.*\:\/\/assets\.nflxext\.com\/.*\/ffe\/player\/html\/.*|)"
                                                  R"(.*\:\/\/www\.assets\.nflxext\.com\/.*\/ffe\/player\/html\/.*)");

        if (netflix1080p_pattern.exactMatch(info.requestUrl().toString()))
        {
            qDebug() << "Netflix Player detected! Injecting Netflix 1080p Unlocker...";
            info.redirect(QUrl("https://rawgit.com/gort818/netflix-1080p/master/cadmium-playercore-5.0008.544.011-1080p.js"));
    }
}
