#ifndef QQMUSIC_H
#define QQMUSIC_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QNetworkProxy>
#include "downloader.h"
#include "daye.h"
#include "config.h"
#include "song.h"
#include "album.h"
#include "playlist.h"
#include "neteasesong.h"
#include "neteaseplaylist.h"
#include "help.h"
#include "thanks.h"
#include "getgray.h"

extern int songNameType;
extern int songQuality;
extern QString mp3FileName;
extern QString mp3Dir;

namespace Ui {
class qqmusic;
}

class qqmusic : public QMainWindow
{
    Q_OBJECT

public:
    explicit qqmusic(QWidget *parent = 0);
    ~qqmusic();
    bool isUrlLegal();
    void doJob();
    void downSinger();
    void downSong();
    void downAlbum();
    void downPlayList();
    bool isUrlGrayLegal();

private slots:
    void nownum(qint32 idx, qint32 total);
    void on_pushButtonDownload_clicked();
    void finished();

    void on_pushButtonChoosePath_clicked();

    void on_radioButtonSongNameType1_clicked();

    void on_radioButtonSongNameType2_clicked();

    void on_radioButtonQuality128_clicked();

    void on_radioButtonQuality320_clicked();

    void on_radioButtonQualityflac_clicked();

    void on_radioButtonQualityape_clicked();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void beginToDownload();

    void neteaseDownloadSong();
    void neteaseeDownloadPlaylist();


    void on_action_triggered();

    void on_action_2_triggered();

    void on_pushButton_clicked();

    void on_pushButtonGray_clicked();

    void on_action_3_triggered();

private:
    Ui::qqmusic *ui;
    QString url;
    QString mid;

    song *m;
    album *a;
    playlist *p;

    neteaseSong *nm;
    neteasePlaylist *np;

    help *h;
    thanks *t;

    getGray *gg;

    QNetworkProxy proxy;

    void doGrayJob();
};

#endif // QQMUSIC_H
