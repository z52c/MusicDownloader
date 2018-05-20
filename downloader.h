#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QtNetwork>
#include <QFile>

class QSslError;

class downloader: public QObject
{
    Q_OBJECT
protected:
    QNetworkAccessManager manager;
    QUrl url;
    QString fileName;
    QNetworkRequest request;
    QNetworkReply *reply;
    QUrlQuery postData;

    int flagSave;
    QString dataStr;

public:
    downloader();
    void init(const QString inUrl,const QString inFileName="");
    void doGet();
    void doPost();
    bool saveToDisk(QIODevice *data);
    static bool isHttpRedirect(QNetworkReply *reply);
    void setUserAgent(const QString ua);
    void setReferer(const QString referer);
    void setCookie(const QString cookie);
    void setOrigin(const QString origin);
    void setAccept(const QString accept);
    QList<QNetworkCookie> getCookies();

    void setPostData(QString key,QString value);
    void clearPostData();

    QString readData();
public slots:
    void downloadFinished(QNetworkReply *reply);
    void sslErrors(const QList<QSslError> &errors);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
signals:
    void progress(qint64, qint64);
    void finished();
    void downloadError(QString);
    void redirected(QString);
};

#endif // DOWNLOADER_H
