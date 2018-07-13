#ifndef SINGERALLALBUM_H
#define SINGERALLALBUM_H

#include <QWidget>
#include "downloadqueue.h"
#include "downloader.h"
#include "config.h"
#include <QFileDialog>

namespace Ui {
class singerAllAlbum;
}

class singerAllAlbum : public QWidget
{
    Q_OBJECT

public:
    explicit singerAllAlbum(QWidget *parent = 0);
    void setDownloadQueue(downloadQueue *inDQ);
    ~singerAllAlbum();

private slots:
    void on_pushButtonChosePath_clicked();

    void on_radioButtonSongNameType1_clicked();

    void on_radioButtonSongNameType2_clicked();

    void on_radioButtonQuality128_clicked();

    void on_radioButtonQuality320_clicked();

    void on_radioButtonQualityflac_clicked();

    void on_radioButtonQualityape_clicked();

    void on_pushButton_clicked();

    void downloadFinished();
    void downloadFailed(QString inStr);

private:
    Ui::singerAllAlbum *ui;

    int songNameType;
    int songQuality;
    QString mp3Dir;

    downloadQueue *dq;
    downloader *d;

    int albumNum;
    int nowNum;


    QString singerMid;
    QStringList albumList;
    QString singerName;

    void doJob();
};

#endif // SINGERALLALBUM_H
