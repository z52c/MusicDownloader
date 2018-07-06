#ifndef VKEYINIT2_H
#define VKEYINIT2_H

#include <QObject>
#include "downloader.h"

extern QString vkey2;

class vkeyinit2 : public QObject
{
    Q_OBJECT
public:
    explicit vkeyinit2(QObject *parent = nullptr);
    void init();
signals:
    void finished();
    void status(QString);
private slots:
    void htmlGot();
    void htmlFailed();
private:
    downloader *d;
};

#endif // VKEYINIT2_H
