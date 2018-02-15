#include "downloader.h"
#include <QDebug>

downloader::downloader()
{
    connect(&manager,SIGNAL(finished(QNetworkReply*)),SLOT(downloadFinished(QNetworkReply*)));
}

downloader::downloader(const QString inUrl,const QString inFileName)
{
    url = QUrl::fromEncoded(inUrl.toLocal8Bit());
    fileName = inFileName;
    connect(&manager,SIGNAL(finished(QNetworkReply*)),SLOT(downloadFinished(QNetworkReply*)));
}

void downloader::init(const QString inUrl, const QString inFileName)
{
    url = QUrl::fromEncoded(inUrl.toLocal8Bit());
    fileName = inFileName;

}

void downloader::setUserAgent(const QString ua)
{
    request.setRawHeader(QByteArray("User-Agent"),QByteArray(ua.toLocal8Bit()));
}

void downloader::setReferer(const QString referer)
{
    request.setRawHeader(QByteArray("Referer"),QByteArray(referer.toLocal8Bit()));
}

void downloader::doDownload()
{
    request.setUrl(url);
    reply = manager.get(request);
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
    return statusCode==320 || statusCode==302 || statusCode==303 || statusCode==305 || statusCode==307 || statusCode==308;
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
        qDebug()<<"download "<<tmpUrl.toEncoded().constData()<<"failed : "<<reply->errorString();
    } else {
        if(isHttpRedirect(reply)){
            qDebug()<<"request was redirected.";
        } else {
            saveToDisk(reply);
        }
        reply->deleteLater();
        emit finished();
    }
}

void downloader::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    emit progress(bytesReceived,bytesTotal);
}
