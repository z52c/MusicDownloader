#ifndef DB4MUSICDOWNLOADER_H
#define DB4MUSICDOWNLOADER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "daye.h"
#include "config.h"
#include <QtDebug>

class db4MusicDownloader : public QObject
{
    Q_OBJECT
public:
    explicit db4MusicDownloader(QObject *parent = nullptr);
    bool insertDownloadQueue(int inFlag,int inQuality,int inNameType,QString inDir,QString inUrl);
    bool deleteFromDownloadQueue(QString inUrl);
    bool _createTable();
    QList<struct downloadQueueInfo> getQueue();
signals:

private:
    QSqlDatabase *db;

};

#endif // DB4MUSICDOWNLOADER_H
