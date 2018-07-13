#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "downloadqueue.h"
#include "help.h"
#include "thanks.h"
#include "vkeyinit2.h"
#include "proxyform.h"
#include <QDesktopServices>
#include <QFileDialog>
#include <QtDebug>
#include "singerallalbum.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonChosePath_clicked();

    void on_radioButtonSongNameType1_clicked();

    void on_radioButtonSongNameType2_clicked();

    void on_radioButtonQuality128_clicked();

    void on_radioButtonQuality320_clicked();

    void on_radioButtonQualityflac_clicked();

    void on_radioButtonQualityape_clicked();

    void status(QString inStr);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void oneTaskFinished(QString inStr);

    void on_pushButtonDownload_clicked();

    void on_pushButtonGray_clicked();

    void init2Finished();

    void on_pushButton_clicked();

    void on_action_triggered();

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_action_4_triggered();

private:
    Ui::MainWindow *ui;
    int songNameType;
    int songQuality;
    QString mp3Dir;

    downloadQueue *dq;

    help *h;
    thanks *t;
    proxyForm *p;
    singerAllAlbum *_singerAllAlbum;
    vkeyinit2 *_vkeyinit2;
};

#endif // MAINWINDOW_H
