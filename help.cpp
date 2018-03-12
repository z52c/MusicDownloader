#include "help.h"


help::help(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::help)
{
    ui->setupUi(this);
    this->hide();
}

help::~help()
{
    delete ui;
}

void help::on_pushButton_clicked()
{
    this->hide();
}
