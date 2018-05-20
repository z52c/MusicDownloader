#include "qqmusicsearch.h"

qqMusicSearch::qqMusicSearch(QObject *parent) : QObject(parent)
{
    d=new downloader();
    connect(d,SIGNAL(downloadError(QString)),this,SLOT(searchResultGotFailed(QString)));
    connect(d,SIGNAL(redirected(QString)),this,SLOT(searchResultGotFailed(QString)));
    connect(d,SIGNAL(finished()),this,SLOT(searchResultGot()));
}

void qqMusicSearch::doJob(QString request)
{
    sRequest=request;
    request=request.toUtf8().toPercentEncoding();
    QString searchUrl="https://c.y.qq.com/soso/fcgi-bin/client_search_cp?ct=24&qqmusic_ver=1298&new_json=1&remoteplace=txt.yqq.center&searchid=46778975652331378&t=0&aggr=1&cr=1&catZhida=1&lossless=0&flag_qc=0&p=1&n=20&w=";
    searchUrl=searchUrl+request+QString("&g_tk=5381&jsonpCallback=MusicJsonCallback20651003929414546&loginUin=0&hostUin=0&format=jsonp&inCharset=utf8&outCharset=utf-8&notice=0&platform=yqq&needNewCode=0");

    d->init(searchUrl,SEARCHRESULTFILE);
    d->doGet();
}

void qqMusicSearch::searchResultGot()
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
        file.close();
        emit finished(0,QString(tmpMid));
    }
}

void qqMusicSearch::searchResultGotFailed(QString errorString)
{
    emit finished(-1,errorString);
}
