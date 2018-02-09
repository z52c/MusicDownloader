#ifndef QQMUSIC_H
#define QQMUSIC_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include "downloader.h"
#include "daye.h"
#include "config.h"
#include "song.h"
#include "album.h"
#include "playlist.h"

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

private:
    Ui::qqmusic *ui;
    QString url;
    QString mid;

    song *m;
    album *a;
    playlist *p;
};

#endif // QQMUSIC_H
