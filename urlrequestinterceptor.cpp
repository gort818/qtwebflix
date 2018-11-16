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
            //info.redirect(QUrl("https://rawgit.com/gort818/netflix-1080p/master/cadmium-playercore-6.0009.325.011-1080p.js"));
            info.redirect(QUrl("https://rawcdn.githack.com/gort818/netflix-1080p/9b99454479a9c4725d22a0f0d74c5f8989f9cf8f/cadmium-playercore-6.0009.457.011-1080p.js"));
    }
}
