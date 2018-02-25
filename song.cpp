#include "song.h"

song::song()
{
    d=new downloader();
    connect(d,SIGNAL(finished()),this,SLOT(shtmlLinkDownloaded()));
    m=new downloader();
    connect(m,SIGNAL(progress(qint64,qint64)),this,SIGNAL(progress(qint64,qint64)));
    connect(m,SIGNAL(finished()),this,SLOT(musicDownloaded()));
    p=new downloader();
    connect(p,SIGNAL(finished()),this,SLOT(picDownloaded()));
    connect(p,SIGNAL(downloadError()),this,SIGNAL(finished()));
}

song::~song()
{
    delete d;
    delete m;
    delete p;
}

song::song(QString mid)
{
    songMid=mid;
    d=new downloader();
    m=new downloader();
    p=new downloader();
    htmlLink=QString(GETVKEYLINKHEAD)+songMid+QString(GETVKEYLINKTAIL);
    connect(d,SIGNAL(finished()),this,SLOT(shtmlLinkDownloaded()));
    connect(m,SIGNAL(progress(qint64,qint64)),this,SIGNAL(progress(qint64,qint64)));
    connect(m,SIGNAL(finished()),this,SIGNAL(musicDownloaded()));
    connect(p,SIGNAL(finished()),this,SLOT(picDownloaded()));
    connect(p,SIGNAL(downloadError()),this,SIGNAL(finished()));
    d->init(htmlLink,QString(SONGHTMLFILE));
    d->setUserAgent(QString(USERAGENT));
    d->doDownload();
}

void song::init(QString mid)
{
    songMid=mid;
    htmlLink=QString(GETVKEYLINKHEAD)+songMid+QString(GETVKEYLINKTAIL);

    d->init(htmlLink,QString(SONGHTMLFILE));
    d->setUserAgent(QString(USERAGENT));
    d->doDownload();
}

void song::shtmlLinkDownloaded()
{
    int flag=0;
    QString line;
    QFile file(SONGHTMLFILE);
    if(file.open(QIODevice::ReadOnly))
    {
        while(!file.atEnd())
        {
            line=file.readLine();
            if(line.contains(QString("window.songlist")))
            {
                flag=1;
                char tmpGuid[200];
                char tmpVkey[200];
                char tmpPicUrl[200];
                char tmpSongName[100];
                char tmpSingerName[100];
                char tmpsizeape[20];
                char tmpMediaMid[20];
                char tmpAlbumName[100];
                getStringBetweenAandB(line.toStdString().c_str(),"songname\":\"","\"",tmpSongName);
                getStringBetweenAandB(line.toStdString().c_str(),"sizeape\":",",",tmpsizeape);
                getStringBetweenAandB(line.toStdString().c_str(),"singername\":\"","\"",tmpSingerName);
                getStringBetweenAandB(line.toStdString().c_str(),"strMediaMid\":\"","\"",tmpMediaMid);
                getStringBetweenAandB(line.toStdString().c_str(),"\"pic\":\"//","\"",tmpPicUrl);
                getStringBetweenAandB(line.toStdString().c_str(),"\"albumname\":\"","\",",tmpAlbumName);

                line=file.readLine();
                getStringBetweenAandB(line.toStdString().c_str(),"guid\":",",",tmpGuid);
                getStringBetweenAandB(line.toStdString().c_str(),"vkey\":\"","\"",tmpVkey);


                mediaMid=QString(tmpMediaMid);
                sizeape=QString(tmpsizeape).toInt();
                singerName=QString(tmpSingerName);
                singerName.remove("\\");
                singerName.remove("/");
                songName=QString(tmpSongName);
                songName.remove("\\");
                songName.remove("/");
                vkey=QString(tmpVkey);
                guid=QString(tmpGuid);
                picUrl=QString("https://")+QString(tmpPicUrl);
                albumName=QString(tmpAlbumName);
                qDebug()<<"sizeape:"<<sizeape;
                downloadSong();
                break;
            }
        }
        file.close();
        if(flag==0)
            emit finished();
    }
}

void song::getSongLink()
{

    if(sizeape==0 || songQuality == 1)
    {
        mp3Link=QString(DOWNLOADLINKHEAD)+QString("M500")+mediaMid+QString(".mp3")+QString(DOWNLOADLINKMID)+vkey+QString(DOWNLOADLINKGUID)+guid+QString(DOWNLOADLINKTAIL);
        return ;
    }


    switch (songQuality) {
    case 2:
        mp3Link=QString(DOWNLOADLINKHEAD)+QString("M800")+mediaMid+QString(".mp3")+QString(DOWNLOADLINKMID)+vkey+QString(DOWNLOADLINKGUID)+guid+QString(DOWNLOADLINKTAIL);
        break;
    case 3:
        mp3Link=QString(DOWNLOADLINKHEAD)+QString("F000")+mediaMid+QString(".flac")+QString(DOWNLOADLINKMID)+vkey+QString(DOWNLOADLINKGUID)+guid+QString(DOWNLOADLINKTAIL);
        break;
    case 4:
        mp3Link=QString(DOWNLOADLINKHEAD)+QString("A000")+mediaMid+QString(".ape")+QString(DOWNLOADLINKMID)+vkey+QString(DOWNLOADLINKGUID)+guid+QString(DOWNLOADLINKTAIL);
        break;
    default:
        mp3Link=QString(DOWNLOADLINKHEAD)+QString("M500")+mediaMid+QString(".mp3")+QString(DOWNLOADLINKMID)+vkey+QString(DOWNLOADLINKGUID)+guid+QString(DOWNLOADLINKTAIL);
        break;
    }
}


void song::downloadSong()
{
    getSongLink();
    if(songNameType==2)
        mp3FileName=mp3Dir+QString("/")+singerName+QString("-")+songName;
    else
        mp3FileName=mp3Dir+QString("/")+songName+QString("-")+singerName;
    switch (songQuality) {
    case 1:
        mp3FileName=mp3FileName+QString("-128")+".mp3";
        break;
    case 2:
        mp3FileName=mp3FileName+".mp3";
        break;
    case 3:
        mp3FileName=mp3FileName+".flac";
        break;
    case 4:
        mp3FileName=mp3FileName+".ape";
        break;
    }
    qDebug()<<"mp3link:"<<mp3Link;
    qDebug()<<"mp3filename:"<<mp3FileName;
    QFile a(mp3FileName);
    if(a.exists())
    {
        qDebug()<<mp3FileName<<"已存在";
        emit finished();
    }
    else
    {
        m->init(mp3Link,mp3FileName);
        emit beginToDownload();
        m->doDownload();
    }
}

void song::musicDownloaded()
{
    if(songQuality==1 || songQuality==2)
    {
        qDebug()<<"准备下载图片";
        qDebug()<<picUrl;
        p->init(picUrl,"tmp.jpg");
        p->doDownload();
    }
    else
    {
        emit finished();
    }

}

void song::picDownloaded()
{
    tagtmp=new ID3tag(mp3FileName);
    connect(tagtmp,SIGNAL(finished()),this,SLOT(tagDone()));
    tagtmp->setTitle(songName);
    tagtmp->setArtist(singerName);
    tagtmp->setAlbum(albumName);
    tagtmp->setPic(QString("tmp.jpg"));
    tagtmp->doJob();
}

void song::tagDone()
{
    delete tagtmp;
    emit finished();
}
