#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QtNetwork>
#include <QFile>

class QSslError;

class downloader: public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager manager;
    QNetworkReply * currentDownload;
    QUrl url;
    QString fileName;
    QNetworkRequest request;
    QNetworkReply *reply;

public:
    downloader();
    downloader(const QString inUrl,const QString inFileName);
    void init(const QString inUrl,const QString inFileName);
    void doDownload();
    bool saveToDisk(QIODevice *data);
    static bool isHttpRedirect(QNetworkReply *reply);
    void setUserAgent(const QString ua);
    void setReferer(const QString referer);
private slots:
    void downloadFinished(QNetworkReply *reply);
    void sslErrors(const QList<QSslError> &errors);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
signals:
    void progress(qint64, qint64);
    void finished();
};

#endif // DOWNLOADER_H
