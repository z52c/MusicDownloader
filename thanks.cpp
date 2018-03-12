#include "thanks.h"
#include "ui_thanks.h"

thanks::thanks(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::thanks)
{
    ui->setupUi(this);
    this->hide();
}

thanks::~thanks()
{
    delete ui;
}

void thanks::on_pushButton_clicked()
{
    this->hide();
}

void thanks::on_pushButton_4_clicked()
{
    this->hide();
}

void thanks::on_pushButton_2_clicked()
{
    this->hide();
}

void thanks::on_pushButton_3_clicked()
{
    this->hide();
}
