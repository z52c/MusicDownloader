#include "qqmusicsong.h"

qqMusicSong::qqMusicSong(QObject *parent) : QObject(parent)
{
    d=new downloader();
    d->setUserAgent(USERAGENT);
    connect(d,SIGNAL(downloadError(QString)),this,SLOT(htmlGotFailed(QString)));
    connect(d,SIGNAL(redirected(QString)),this,SLOT(htmlGotFailed(QString)));
    connect(d,SIGNAL(finished()),this,SLOT(htmlGot()));

    m=new downloader();
    connect(m,SIGNAL(downloadError(QString)),this,SLOT(musicDownloadFailed(QString)));
    connect(m,SIGNAL(redirected(QString)),this,SLOT(musicDownloadFailed(QString)));
    connect(m,SIGNAL(finished()),this,SLOT(musicDownloaded()));
    connect(m,SIGNAL(progress(qint64,qint64)),this,SIGNAL(progress(qint64,qint64)));

    p=new downloader();
    connect(p,SIGNAL(downloadError(QString)),this,SLOT(picDownloadFailed(QString)));
    connect(p,SIGNAL(redirected(QString)),this,SLOT(picDownloadFailed(QString)));
    connect(p,SIGNAL(finished()),this,SLOT(picDownloaded()));
}

void qqMusicSong::doJob(QString inMid)
{
    sizeape=0;
    size320=0;
    songMid=inMid;
    htmlLink=QString(SINGLESONGHEAD)+songMid+QString(SINGLESONGTAIL);

    d->init(htmlLink,QString(SONGINFOFILE));
    d->doGet();
}

void qqMusicSong::htmlGotFailed(QString errorString)
{
    emit status(songMid+QString("-html下载失败，失败原因：")+errorString);
    finished(-1);
}


/****************************
 * html文件已经下载
 * 找出mediaMid,songName,albumMid,singerName
 * **************************/

void qqMusicSong::htmlGot()
{
    QFile file(SONGINFOFILE);
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray b=file.readAll();
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(b,&jsonError);
        if(jsonError.error!=QJsonParseError::NoError)
        {
            qDebug() << "read singleSong json file failed";
            emit("read singleSong json file failed");
            file.close();
        }
        QJsonObject obj=doucment.object();
        QJsonArray tmpArray=obj["data"].toArray();
        obj=tmpArray.first().toObject();

        QJsonObject tmpObj=obj["album"].toObject();
        albumMid=tmpObj["mid"].toString();
        qDebug()<<albumMid;
        albumName=tmpObj["name"].toString();
        qDebug()<<albumName;
        picUrl=QString(ALBUMPICLINKHEAD)+albumMid+QString(ALBUMPICLINKTAIL);
        qDebug()<<picUrl;
        songName=obj["name"].toString();
        songName.remove("\\");
        singerName.remove("/");
        qDebug()<<songName;
        tmpObj=obj["file"].toObject();
        sizeape=tmpObj["size_ape"].toInt();
        size320=tmpObj["size_320mp3"].toInt();
        qDebug()<<size320;
        mediaMid=tmpObj["media_mid"].toString();
        qDebug()<<mediaMid;
        tmpArray=obj["singer"].toArray();
        tmpObj=tmpArray.first().toObject();
        singerName=tmpObj["name"].toString();
        singerName.remove("\\");
        singerName.remove("/");
        qDebug()<<singerName;
        tmpArray.removeFirst();
        while(tmpArray.count()>0)
        {
            tmpObj=tmpArray.first().toObject();
            singerName=singerName+QString("-")+tmpObj["name"].toString();
            tmpArray.removeFirst();
            qDebug()<<singerName;
        }


        file.close();
        downloadSong();
    }
}

void qqMusicSong::musicDownloadFailed(QString errorString)
{
    emit status(QString("下载失败：")+errorString);
    finished(-2);
}

void qqMusicSong::musicDownloaded()
{
    if(songQuality==1 || songQuality==2)
    {
        qDebug()<<"准备下载图片";
        qDebug()<<picUrl;
        emit status("准备下载封面图片");
        p->init(picUrl,"tmp.jpg");
        p->doGet();
    }
    else
    {
        emit finished(0);
    }
}

void qqMusicSong::picDownloadFailed(QString errorString)
{
    emit status(QString("封面图片下载失败:")+errorString+QString(",不设置封面图"));
    finished(0);
}

void qqMusicSong::picDownloaded()
{
    tagtmp=new ID3tag(mp3FileName);
    connect(tagtmp,SIGNAL(finished()),this,SLOT(tagDone()));
    tagtmp->setTitle(songName);
    tagtmp->setArtist(singerName);
    tagtmp->setAlbum(albumName);
    tagtmp->setPic(QString("tmp.jpg"));
    tagtmp->doJob();
}

void qqMusicSong::tagDone()
{
    delete tagtmp;
    emit status("封面图设置结束");
    emit finished(0);
}

void qqMusicSong::getSongLink()
{
    if(size320==0 || songQuality == 1)
    {
        mp3Link=QString(DOWNLOADLINKHEAD)+QString("M500")+mediaMid+QString(".mp3")+QString(DOWNLOADLINKMID)+vkey+QString(DOWNLOADLINKGUID)+guid+QString(DOWNLOADLINKTAIL);
        return ;
    }

    if(sizeape==0 || songQuality == 2)
    {
        mp3Link=QString(DOWNLOADLINKHEAD)+QString("M800")+mediaMid+QString(".mp3")+QString(DOWNLOADLINKMID)+vkey+QString(DOWNLOADLINKGUID)+guid+QString(DOWNLOADLINKTAIL);
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

void qqMusicSong::downloadSong()
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
        emit status(mp3FileName+QString("已经存在"));
        emit finished(0);
    }
    else
    {
        m->init(mp3Link,mp3FileName);
        emit status(QString("准备下载")+mp3FileName);
        m->doGet();
    }
}
