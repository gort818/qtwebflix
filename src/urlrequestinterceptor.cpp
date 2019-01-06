#include <QWebEngineUrlRequestInterceptor>
#include <QDebug>

#include "urlrequestinterceptor.h"

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

            // old playercore still works but a lot fo shows no longer play in 1080
            //info.redirect(QUrl("https://rawcdn.githack.com/gort818/netflix-1080p/a225d19994546396f252a169704e2bde43e5ff7d/playercore-481.js"));
            //new playercore (ctrl + alt + shift + s no longer working
            info.redirect(QUrl("https://rawcdn.githack.com/gort818/netflix-1080p/15c20e1d1880cc19414840d413e940c34b1bb438/playercore-6.0011.853.051.js"));
    }
}
