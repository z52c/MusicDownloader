#ifndef VKEYINIT_H
#define VKEYINIT_H

#include <QObject>
#include "downloader.h"
#include "daye.h"
#include "config.h"

extern int songNameType;
extern int songQuality;
extern QString mp3FileName;
extern QString mp3Dir;
extern QString vkey;
extern QString guid;


class vkeyInit : public QObject
{
    Q_OBJECT
public:
    explicit vkeyInit(QObject *parent = nullptr);
    void init();
signals:
    void finished();
    void status(QString);
private slots:
    void htmlGot();
    void htmlFailed();
private:
    QStringList tmpSongMidList;
    downloader *d;
};

#endif // VKEYINIT_H
