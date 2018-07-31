#ifndef NETEASEPLAYLIST_H
#define NETEASEPLAYLIST_H

#include <QObject>
#include "downloader.h"
#include "daye.h"
#include "config.h"
#include "neteasesong.h"

extern int songNameType;
extern int songQuality;
extern QString mp3FileName;
extern QString mp3Dir;
extern QString vkey;
extern QString guid;


class neteasePlaylist : public QObject
{
    Q_OBJECT
public:
    explicit neteasePlaylist(QObject *parent = nullptr);
    void doJob(bool inIsGray,QString inUrl);
signals:
    void finished(int ,QStringList);
    void status(QString);
public slots:
    void htmlGot();
    void htmlGotFailed(QString errorString);
    void htmlGotRedirected(QString redirectString);
    void neteaseSongFinished(int inFlag,bool inIsGray,QString inString);

private:
    QString mid;
    downloader *d;
    neteaseSong *s;
    QStringList songMidList;
    bool isGray;
    QStringList infoList;
    QStringList grayList;
    QString listTitle;
};

#endif // NETEASEPLAYLIST_H
