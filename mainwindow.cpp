#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{   songNameType=0;
    songQuality=0;
    ui->setupUi(this);
    dq=new downloadQueue();
    connect(dq,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));
    connect(dq,SIGNAL(status(QString)),this,SLOT(status(QString)));
    connect(dq,SIGNAL(finished(QString)),this,SLOT(oneTaskFinished(QString)));

    h=new help();
    t=new thanks();
    p=new proxyForm();
    _singerAllAlbum=new singerAllAlbum();
    _singerAllAlbum->setDownloadQueue(dq);

    _vkeyinit2=new vkeyinit2();
    connect(_vkeyinit2,SIGNAL(status(QString)),this,SLOT(status(QString)));
    connect(_vkeyinit2,SIGNAL(finished()),this,SLOT(init2Finished()));

    ui->pushButtonDownload->setEnabled(false);
    _vkeyinit2->init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init2Finished()
{
    ui->pushButtonDownload->setEnabled(true);
    QList<struct downloadQueueInfo> tmp;
    tmp=dq->getQueue();
    while(tmp.count()>0)
    {
        ui->textEditDownloadQueueInfo->append(tmp.at(0).url);
        tmp.removeAt(0);
    }
}

void MainWindow::on_pushButtonChosePath_clicked()
{
    mp3Dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->lineEditMp3Path->setText(mp3Dir);
}


void MainWindow::on_radioButtonSongNameType1_clicked()
{
    songNameType=1;
}

void MainWindow::on_radioButtonSongNameType2_clicked()
{
    songNameType=2;
}

void MainWindow::on_radioButtonQuality128_clicked()
{
    songQuality=1;
}

void MainWindow::on_radioButtonQuality320_clicked()
{
    songQuality=2;
}

void MainWindow::on_radioButtonQualityflac_clicked()
{
    songQuality=3;
}

void MainWindow::on_radioButtonQualityape_clicked()
{
    songQuality=4;
}

void MainWindow::on_pushButtonDownload_clicked()
{
    mp3Dir=ui->lineEditMp3Path->text();
    if(!songNameType || !songQuality || ui->lineEditMp3Path->text().isEmpty() || ui->lineEditLink->text().isEmpty())
    {
        ui->textEditFinishedInfo->append("请输入合法的设置");
    }
    else{
        if(ui->lineEditLink->text().contains("y.qq.com"))
        {
            if(ui->lineEditLink->text().contains("num"))
            {
                ui->textEditFinishedInfo->append("请选择不含num的链接");
                return;
            }
            qDebug()<<QString("quality:%1").arg(songQuality);
            dq->insertQueue(0,songQuality,songNameType,mp3Dir,ui->lineEditLink->text());
            ui->textEditDownloadQueueInfo->append(ui->lineEditLink->text());

        }
        else{
            if(ui->lineEditLink->text().contains("music.163.com"))
            {
                if(ui->lineEditLink->text().contains("playlist"))
                {
                    ui->textEditDownloadQueueInfo->append(ui->lineEditLink->text());
                    dq->insertQueue(1,songQuality,songNameType,mp3Dir,ui->lineEditLink->text());
                }
                else{
                    ui->textEditFinishedInfo->append("网易云音乐仅支持歌单下载，单曲以及专辑下载请使用qq音乐链接");
                }
            }
        }

    }
}


void MainWindow::status(QString inStr)
{
    ui->textEditNowInfo->append(inStr);
}

void MainWindow::oneTaskFinished(QString inStr)
{
    ui->textEditDownloadQueueInfo->clear();
    ui->textEditNowInfo->clear();
    ui->textEditFinishedInfo->append(QString("%1 finished").arg(inStr));
    QList<struct downloadQueueInfo> tmp;
    tmp=dq->getQueue();
    while(tmp.count()>0)
    {
        ui->textEditDownloadQueueInfo->append(tmp.at(0).url);
        tmp.removeAt(0);
    }
}

void MainWindow::on_pushButtonGray_clicked()
{
    mp3Dir=ui->lineEditMp3Path->text();
    if(ui->lineEditLink->text().contains("music.163.com")&&ui->lineEditLink->text().contains("playlist"))
    {
        ui->textEditDownloadQueueInfo->append(ui->lineEditLink->text());
        dq->insertQueue(2,songNameType,songQuality,mp3Dir,ui->lineEditLink->text());
    }
}

void MainWindow::on_pushButton_clicked()
{
    qDebug()<<mp3Dir;
    QDesktopServices::openUrl(QUrl::fromLocalFile(mp3Dir));
}

void MainWindow::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    ui->progressBar->setMaximum(bytesTotal);
    ui->progressBar->setValue(bytesReceived);
}

void MainWindow::on_action_triggered()
{
    h->show();
}

void MainWindow::on_action_2_triggered()
{
    t->show();
}

void MainWindow::on_action_3_triggered()
{
    p->show();
}

void MainWindow::on_action_4_triggered()
{
    _singerAllAlbum->show();
}
