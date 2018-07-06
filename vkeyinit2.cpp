#include "vkeyinit2.h"
#include "daye.h"
#include "config.h"
#include <QtDebug>

vkeyinit2::vkeyinit2(QObject *parent) : QObject(parent)
{
    d=new downloader();
    connect(d,SIGNAL(finished()),this,SLOT(htmlGot()));
    connect(d,SIGNAL(downloadError(QString)),this,SLOT(htmlFailed()));
}

void vkeyinit2::init()
{
    QString htmlLink=QString("https://c.y.qq.com/base/fcgi-bin/fcg_music_express_mobile3.fcg?g_tk=0&loginUin=1008611&hostUin=0&format=json&inCharset=utf8&outCharset=utf-8&notice=0&platform=yqq&needNewCode=0&cid=205361747&uin=1008611&songmid=003a1tne1nSz1Y&filename=C400003a1tne1nSz1Y.m4a&guid=1234567890");
    d->init(htmlLink,QString(SONGHTMLFILE));
    d->setUserAgent("Dalvik/2.1.0 (Linux; U; Android 5.1.1; vivo x5s l Build/LMY48Z)");
    d->setReferer("https://y.qq.com/portal/profile.html");
    d->doGet();
}

void vkeyinit2::htmlFailed()
{
    emit status("无法下载无损歌曲");
}

void vkeyinit2::htmlGot()
{
    QString line;
    QFile file(SONGHTMLFILE);
    int a;
    char tmp[200];
    if(file.open(QIODevice::ReadOnly))
    {
        line=file.readLine();
        if(line.contains("vkey"))
        {
            a=getStringBetweenAandB(line.toStdString().c_str(),"vkey\":\"","\"",tmp);
            if(!a && strlen(tmp))
            {
                vkey2=QString(tmp);
                qDebug()<<vkey2;
                emit finished();
            }
        }
        file.close();
    }
}
