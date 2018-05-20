#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qqmusic.h"
#include <QFileDialog>
#include "neteaseplaylist.h"
#include "help.h"
#include "thanks.h"
#include <QNetworkProxy>
#include "proxyform.h"

extern int songNameType;
extern int songQuality;
extern QString mp3FileName;
extern QString mp3Dir;
extern QString vkey;
extern QString guid;


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
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void qqmusicFinished();
    void status(QString inString);

    void on_pushButtonDownload_clicked();

    void on_radioButtonSongNameType1_clicked();

    void on_radioButtonSongNameType2_clicked();

    void on_radioButtonQuality128_clicked();

    void on_radioButtonQuality320_clicked();

    void on_radioButtonQualityflac_clicked();

    void on_radioButtonQualityape_clicked();

    void on_pushButtonChosePath_clicked();

    void on_pushButtonGray_clicked();

    void on_action_triggered();

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    qqmusic *q;
    neteasePlaylist *n;
    help *h;
    thanks *t;
    proxyForm *p;

    QString proxyAddr;
    QString proxyPort;
    QString proxyUser;
    QString proxyPass;

    QNetworkProxy proxy;
};

#endif // MAINWINDOW_H
