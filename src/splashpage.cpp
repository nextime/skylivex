#include "skylivex.h"
#include <QFile>
#include <QDir>

SplashPage::SplashPage(QFile &splash_html)
      : QWebView(0)
{
   splash_html.open(QIODevice::ReadOnly);
   splashFile = QString::fromUtf8(splash_html.readAll().constData());
   QUrl baseUrl = QUrl::fromLocalFile(QDir::current().absoluteFilePath("gui/dummy.html"));

   setWindowFlags(Qt::FramelessWindowHint);
   page()->setPalette(palette());
   setAttribute(Qt::WA_TranslucentBackground, true);
   setAttribute(Qt::WA_OpaquePaintEvent, false);

   setHtml(splashFile, baseUrl);
   resize(250,200);
}

SplashPage::~SplashPage()
{

}
