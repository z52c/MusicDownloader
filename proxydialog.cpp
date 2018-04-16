#include "proxydialog.h"
#include "ui_proxydialog.h"

proxyDialog::proxyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::proxyDialog)
{
    ui->setupUi(this);
}

proxyDialog::~proxyDialog()
{
    delete ui;
}


QStringList proxyDialog::getInput()
{
    QStringList a;
    a.append(ui->lineEditAdd->text());
    a.append(ui->lineEditPort->text());
    a.append(ui->lineEditUser->text());
    a.append(ui->lineEditPass->text());
    return a;
}
