#ifndef QQMUSICSEARCH_H
#define QQMUSICSEARCH_H

#include <QObject>
#include <downloader.h>
#include "config.h"
#include "daye.h"

class qqMusicSearch : public QObject
{
    Q_OBJECT
public:
    explicit qqMusicSearch(QObject *parent = nullptr);
    void doJob(QString request);
signals:
    void finished(int,QString);
public slots:
    void searchResultGot();
    void searchResultGotFailed(QString errorString);
private:
    downloader *d;
    QString sRequest;
};

#endif // QQMUSICSEARCH_H
