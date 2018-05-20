#include "neteasesong.h"

neteaseSong::neteaseSong(QObject *parent) : QObject(parent)
{
    d=new downloader();
    d->setUserAgent(UA);
    connect(d,SIGNAL(finished()),this,SLOT(htmlFileGot()));
    connect(d,SIGNAL(downloadError(QString)),this,SLOT(htmlFileGotFailed(QString)));
    connect(d,SIGNAL(redirected(QString)),this,SLOT(htmlFileGotFailed(QString)));

    search=new qqMusicSearch();
    connect(search,SIGNAL(finished(int,QString)),this,SLOT(searchResultGot(int,QString)));
}

void neteaseSong::doJob(QString inMid)
{
    QString tmpUrl=QString("http://music.163.com/song?id=")+inMid;
    qDebug()<<tmpUrl;
    d->init(tmpUrl,QString(SONGHTMLFILE));
    d->doGet();
}


void neteaseSong::htmlFileGotFailed(QString errorString)
{
    emit finished(-1,0,errorString);
}

void neteaseSong::htmlFileGot()
{
    isGray=false;
    QString line;
    QFile file(SONGHTMLFILE);
    file.open(QIODevice::ReadOnly);
    char tmpSongName[200];
    char tmpSinger[100];
    char tmpAlbum[100];
    while(!file.atEnd())
    {
        line=file.readLine();

        if(line.contains(QString("appid")))
        {
            line=file.readLine();
            getStringBetweenAandB(line.toStdString().c_str(),"\"title\": \"","\"",tmpSongName);
            line=file.readLine();
            line=file.readLine();
            getStringBetweenAandB(line.toStdString().c_str(),"歌手：","。",tmpSinger);
            getStringBetweenAandB(line.toStdString().c_str(),"所属专辑：","。",tmpAlbum);
            break;
        }
    }
    while(!file.atEnd())
    {
        line=file.readLine();
        if(line.contains("u-btni-play"))
        {
            isGray=true;
            break;
        }
    }
    file.close();
    search->doJob(QString(tmpSongName)+QString(" ")+QString(tmpSinger)+QString(" ")+QString(tmpAlbum));
}

void neteaseSong::searchResultGot(int inFlag, QString inString)
{
    finished(inFlag,isGray,inString);
}
