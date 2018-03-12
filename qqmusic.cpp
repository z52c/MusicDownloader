#include "qqmusic.h"
#include "ui_qqmusic.h"
#include <QDesktopServices>

qqmusic::qqmusic(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::qqmusic)
{
    ui->setupUi(this);
    h=new help(this);
    t=new thanks(this);
    songNameType=0;
    songQuality=0;
    m=new song;
    connect(m,SIGNAL(progress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));
    connect(m,SIGNAL(finished()),this,SLOT(finished()));
    connect(m,SIGNAL(beginToDownload()),this,SLOT(beginToDownload()));

    a=new album();
    connect(a,SIGNAL(beginToDownload()),this,SLOT(beginToDownload()));
    connect(a,SIGNAL(finished()),this,SLOT(finished()));
    connect(a,SIGNAL(progress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));
    connect(a,SIGNAL(nownum(qint32,qint32)),this,SLOT(nownum(qint32,qint32)));

    p=new playlist();
    connect(p,SIGNAL(beginToDownload()),this,SLOT(beginToDownload()));
    connect(p,SIGNAL(finished()),this,SLOT(finished()));
    connect(p,SIGNAL(progress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));
    connect(p,SIGNAL(nownum(qint32,qint32)),this,SLOT(nownum(qint32,qint32)));

    nm=new neteaseSong();
    connect(nm,SIGNAL(finished()),this,SLOT(finished()));
    connect(nm,SIGNAL(beginToDownload()),this,SLOT(beginToDownload()));
    connect(nm,SIGNAL(progress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));

    np=new neteasePlaylist();
    connect(np,SIGNAL(finished()),this,SLOT(finished()));
    connect(np,SIGNAL(beginToDownload()),this,SLOT(beginToDownload()));
    connect(np,SIGNAL(progress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));
    connect(np,SIGNAL(nownum(qint32,qint32)),this,SLOT(nownum(qint32,qint32)));
}

qqmusic::~qqmusic()
{
    delete ui;
    delete m;
    delete a;
    delete p;
    delete nm;
    delete np;
}

void qqmusic::on_pushButtonDownload_clicked()
{
    if(songNameType == 0 || songQuality ==0)
    {
        ui->labelStatus->setText("请选择名称格式以及音质.");
        return;
    }
    url=ui->lineEditLink->text();
    if(!isUrlLegal())
    {
        ui->labelStatus->setText("链接不合法，请检查链接.");
        return ;
    }
    if(QString(ui->lineEditLink->text()).isEmpty())
    {
        ui->labelStatus->setText("请输入链接.");
        return;
    }
    if(QString(ui->lineEditMp3Path->text()).isEmpty())
    {
        ui->labelStatus->setText("请选择保存目录.");
        return;
    }
    if(!isUrlLegal())
    {
        ui->labelStatus->setText("链接不合法，请重新输入.");
        return;
    }
    QFileInfo tmpfileinfo(mp3Dir);
    if(!tmpfileinfo.isDir())
    {
        ui->labelStatus->setText("请选择正确的保存目录.");
        return;
    }
    ui->lineEditLink->setEnabled(false);
    ui->lineEditMp3Path->setEnabled(false);
    ui->pushButtonChoosePath->setEnabled(false);
    ui->pushButtonDownload->setEnabled(false);
    ui->labelStatus->setText("准备开始下载...");
    mp3Dir=ui->lineEditMp3Path->text();
    doJob();
}


void qqmusic::on_pushButtonChoosePath_clicked()
{
    mp3Dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->lineEditMp3Path->setText(mp3Dir);
}


void qqmusic::on_radioButtonSongNameType1_clicked()
{
    songNameType=1;
}

void qqmusic::on_radioButtonSongNameType2_clicked()
{
    songNameType=2;
}

void qqmusic::on_radioButtonQuality128_clicked()
{
    songQuality=1;
}

void qqmusic::on_radioButtonQuality320_clicked()
{
    songQuality=2;
}

void qqmusic::on_radioButtonQualityflac_clicked()
{
    songQuality=3;
}

void qqmusic::on_radioButtonQualityape_clicked()
{
    songQuality=4;
}

bool qqmusic::isUrlLegal()
{
    return(url.contains("y.qq.com")||url.contains("music.163.com"));
}

void qqmusic::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    ui->progressBar->setMaximum(bytesTotal);
    ui->progressBar->setValue(bytesReceived);
}

void qqmusic::doJob()
{
    char tmpMid[20];
    int pos;
    if(url.contains("y.qq.com/n/yqq/song"))
    {
        getStringBetweenAandB(url.toStdString().c_str(),"y.qq.com/n/yqq/song/",".html",tmpMid);
        mid=QString(tmpMid);
        downSong();
        return;
    }

    if(url.contains("y.qq.com/n/yqq/album"))
    {
        getStringBetweenAandB(url.toStdString().c_str(),"y.qq.com/n/yqq/album/",".html",tmpMid);
        mid=QString(tmpMid);
        downAlbum();
        return;
    }
  /*  if(url.contains("y.qq.com/n/yqq/singer"))
    {
        getStringBetweenAandB(url.toStdString().c_str(),"y.qq.com/n/yqq/singer/",".html",tmpMid);
        mid=QString(tmpMid);
        downSinger();
        return;
    }*/
    if(url.contains("y.qq.com/n/yqq/playsquare"))
    {
        getStringBetweenAandB(url.toStdString().c_str(),"y.qq.com/n/yqq/playsquare/",".html",tmpMid);
        mid=QString(tmpMid);
        downPlayList();
        return;
    }
    if(url.contains("y.qq.com/n/yqq/playlist"))
    {
        getStringBetweenAandB(url.toStdString().c_str(),"y.qq.com/n/yqq/playlist/",".html",tmpMid);
        mid=QString(tmpMid);
        downPlayList();
        return;
    }

    if(url.contains("song?id=")&&url.contains("music.163.com"))
    {
        if(url.contains("&"))
        {
            char a[20];
            getStringBetweenAandB(url.toStdString().c_str(),"=","&",a);
            mid=QString(a);
        }else{
            pos=url.indexOf(QString("="));
            mid=url.mid(pos+1);
        }
        neteaseDownloadSong();
        return;
    }

    if(url.contains("playlist?id=")&&url.contains("music.163.com"))
    {
        if(url.contains("&"))
        {
            char a[20];
            getStringBetweenAandB(url.toStdString().c_str(),"=","&",a);
            mid=QString(a);
        }else{
            pos=url.indexOf(QString("="));
            mid=url.mid(pos+1);
        }
        neteaseeDownloadPlaylist();
        return;
    }

    ui->lineEditLink->setEnabled(true);
    ui->lineEditMp3Path->setEnabled(true);
    ui->pushButtonChoosePath->setEnabled(true);
    ui->pushButtonDownload->setEnabled(true);
    ui->labelStatus->setText("链接不合法，请重新输入...");
}

void qqmusic::downSong()
{
    m->init(mid);
}


void qqmusic::downAlbum()
{

    qDebug()<<"reay for allbum";
    a->init(mid);
}

void qqmusic::downPlayList()
{
    p->init(mid);
}

void qqmusic::downSinger()
{

}

void qqmusic::neteaseDownloadSong()
{
    qDebug()<<mid;

    nm->init(mid);
}

void qqmusic::neteaseeDownloadPlaylist()
{
    np->init(mid);
}

void qqmusic::finished()
{
    ui->lineEditLink->setEnabled(true);
    ui->lineEditMp3Path->setEnabled(true);
    ui->pushButtonChoosePath->setEnabled(true);
    ui->pushButtonDownload->setEnabled(true);
    ui->pushButtonDownload->setEnabled(true);
    ui->labelStatus->setText(mp3FileName+QString("下载结束..."));
}

void qqmusic::beginToDownload()
{
    ui->labelStatus->setText(mp3FileName+QString("开始下载..."));
}

void qqmusic::nownum(qint32 idx, qint32 total)
{
    qDebug()<<idx<<" "<<total;
    QString a=QString("当前第")+QString::number(idx)+QString("首，总")+QString::number(total)+QString("首");
    ui->labelNowNum->setText(a);
}

void qqmusic::on_action_triggered()
{

    h->show();
}

void qqmusic::on_action_2_triggered()
{
    t->show();
}

void qqmusic::on_pushButton_clicked()
{
    QDesktopServices::openUrl(QUrl(mp3Dir.toStdString().c_str(),QUrl::TolerantMode));
}
