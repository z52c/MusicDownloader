#include "neteasesong.h"
#include "config.h"
#include "daye.h"

neteaseSong::neteaseSong()
{
    d=new downloader();
    connect(d,SIGNAL(finished()),this,SLOT(htmlDownloaded()));

    search=new downloader();
    connect(search,SIGNAL(finished()),this,SLOT(searchDownloaded()));

    a=new song();
    connect(a,SIGNAL(beginToDownload()),this,SIGNAL(beginToDownload()));
    connect(a,SIGNAL(progress(qint64,qint64)),this,SIGNAL(progress(qint64,qint64)));
    connect(a,SIGNAL(finished()),this,SIGNAL(finished()));
}

void neteaseSong::init(QString mid)
{
    QString tmpUrl=QString("http://music.163.com/song?id=")+mid;
    qDebug()<<tmpUrl;
    d->init(tmpUrl,QString(SONGHTMLFILE));
    d->doDownload();
}

void neteaseSong::htmlDownloaded()
{
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
    file.close();
    singer=QString(tmpSinger);
    songName=QString(tmpSongName);
    album=QString(tmpAlbum);
    qDebug()<<singer+songName+album;

    songInfoGot();
}


void neteaseSong::songInfoGot()
{
    QString request=songName+QString(" ")+singer+QString(" ")+album;
    request=request.toUtf8().toPercentEncoding();
    QString searchUrl="https://c.y.qq.com/soso/fcgi-bin/client_search_cp?ct=24&qqmusic_ver=1298&new_json=1&remoteplace=txt.yqq.center&searchid=46778975652331378&t=0&aggr=1&cr=1&catZhida=1&lossless=0&flag_qc=0&p=1&n=20&w=";
    searchUrl=searchUrl+request+QString("&g_tk=5381&jsonpCallback=MusicJsonCallback20651003929414546&loginUin=0&hostUin=0&format=jsonp&inCharset=utf8&outCharset=utf-8&notice=0&platform=yqq&needNewCode=0");

    search->init(searchUrl,SEARCHRESULTFILE);
    search->doDownload();

}

void neteaseSong::searchDownloaded()
{
    QFile file(SEARCHRESULTFILE);
    QString line;
    char tmpMid[30];
    if(file.open(QIODevice::ReadOnly))
    {
        line=file.readLine();
        int pos=line.indexOf(QString("lyric"));
        line=line.mid(pos);
        getStringBetweenAandB(line.toStdString().c_str(),"mid\":\"","\"",tmpMid);

    }
    file.close();
    qDebug()<<QString("search result:")+tmpMid;
    a->init(QString(tmpMid));
}

neteaseSong::~neteaseSong()
{
    delete d;
}
