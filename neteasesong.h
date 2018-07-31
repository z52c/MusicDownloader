#ifndef NETEASESONG_H
#define NETEASESONG_H

#include <QObject>
#include "downloader.h"
#include "daye.h"
#include "config.h"
#include "qqmusicsearch.h"

class neteaseSong : public QObject
{
    Q_OBJECT
public:
    explicit neteaseSong(QObject *parent = nullptr);
    void doJob(QString inMid);
signals:
    void finished(int ,bool,QString);
public slots:
    void htmlFileGot();
    void htmlFileGotFailed(QString errorString);
    void htmlGotRedirected(QString redirectString);
    void searchResultGot(int inFlag,QString inString);
private:
    downloader *d;
    qqMusicSearch *search;
    bool isGray;
};

#endif // NETEASESONG_H
