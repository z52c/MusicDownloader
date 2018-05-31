#include "downloader.h"
#include <QtDebug>

downloader::downloader()
{
    manager.setCookieJar(new QNetworkCookieJar(this));
    connect(&manager,SIGNAL(finished(QNetworkReply*)),SLOT(downloadFinished(QNetworkReply*)));
}

void downloader::init(const QString inUrl, const QString inFileName)
{
    if(inFileName=="")
    {
        flagSave=0;
        url = QUrl::fromEncoded(inUrl.toLocal8Bit());
    }
    else
    {
        flagSave=1;
        url = QUrl::fromEncoded(inUrl.toLocal8Bit());
        fileName = inFileName;
    }
}
/*
void downloader::init(char *inU, char *inF=NULL)
{
    if(inF==NULL)
    {
        init(QString(inU));
    }
}
*/
void downloader::setUserAgent(const QString ua)
{
    request.setRawHeader(QByteArray("User-Agent"),QByteArray(ua.toLocal8Bit()));
}

void downloader::setReferer(const QString referer)
{
    request.setRawHeader(QByteArray("Referer"),QByteArray(referer.toLocal8Bit()));
}

void downloader::setCookie(const QString cookie)
{
    request.setRawHeader(QByteArray("Cookie"),QByteArray(cookie.toLocal8Bit()));
}

void downloader::setOrigin(const QString origin)
{
    request.setRawHeader(QByteArray("Origin"),QByteArray(origin.toLocal8Bit()));
}

void downloader::setAccept(const QString accept)
{
    request.setRawHeader(QByteArray("Accept"),QByteArray(accept.toLocal8Bit()));
}

void downloader::setPostData(QString key, QString value)
{
    postData.addQueryItem(key,value);
}

void downloader::clearPostData()
{
    postData.clear();
}

void downloader::doGet()
{
    request.setUrl(url);
    reply = manager.get(request);
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));
#if QT_CONFIG(ssl)
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
            SLOT(sslErrors(QList<QSslError>)));
#endif
}

void downloader::doPost()
{
    request.setUrl(url);
    qDebug()<<url;
    reply = manager.post(request,postData.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));
#if QT_CONFIG(ssl)
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
            SLOT(sslErrors(QList<QSslError>)));
#endif
}

bool downloader::saveToDisk(QIODevice *data)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug()<<"could not open file to write";
        return false;
    }
    file.write(data->readAll());
    file.close();
    qDebug()<<"66666"<<fileName;
    return true;
}

bool downloader::isHttpRedirect(QNetworkReply *reply)
{

    int statusCode=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode==320 || statusCode==302 || statusCode==303 || statusCode==305 || statusCode==307 || statusCode==308 || statusCode==301;
}

void downloader::sslErrors(const QList<QSslError> &errors)
{
#if QT_CONFIG(ssl)
    //for (const QSslError &error : sslErrors)
    //    qDebug()<<"ssl error:"<<error.errorString();
#else
    //Q_UNUSED(sslErrors);
#endif
}

void downloader::downloadFinished(QNetworkReply *reply)
{
    QUrl tmpUrl=reply->url();
    if(reply->error())
    {
        QString errorStringTmp=QString("download ")+tmpUrl.toEncoded().constData()+QString("failed : ")+reply->errorString();
        qDebug()<<errorStringTmp;
        emit downloadError(errorStringTmp);
    } else {
        if(isHttpRedirect(reply)){
            qDebug()<<"request was redirected.";
            qDebug()<<reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            reply->deleteLater();
            emit redirected(reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString());
        } else {
            if(flagSave==0)
            {
                dataStr=reply->readAll();
                reply->deleteLater();
                emit finished();
            }
            else{
                saveToDisk(reply);
                reply->deleteLater();
                emit finished();
            }
        }
    }
}

void downloader::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    emit progress(bytesReceived,bytesTotal);
}


QList<QNetworkCookie> downloader::getCookies()
{
    return manager.cookieJar()->cookiesForUrl(url);
}


QString downloader::readData()
{
    return dataStr;
}
