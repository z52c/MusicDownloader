#ifndef PROXYDIALOG_H
#define PROXYDIALOG_H

#include <QDialog>

namespace Ui {
class proxyDialog;
}

class proxyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit proxyDialog(QWidget *parent = 0);
    ~proxyDialog();
    QStringList getInput();

private:
    Ui::proxyDialog *ui;
};

#endif // PROXYDIALOG_H
