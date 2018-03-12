#ifndef HELP_H
#define HELP_H

#include <QWidget>
#include "ui_help.h"

namespace Ui {
class help;
}

class help : public QWidget
{
    Q_OBJECT

public:
    explicit help(QWidget *parent = 0);
    ~help();

private slots:
    void on_pushButton_clicked();

private:
    Ui::help *ui;
};

#endif // HELP_H
