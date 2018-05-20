#ifndef PROXYFORM_H
#define PROXYFORM_H

#include <QWidget>

namespace Ui {
class proxyForm;
}

class proxyForm : public QWidget
{
    Q_OBJECT

public:
    explicit proxyForm(QWidget *parent = 0);
    ~proxyForm();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::proxyForm *ui;
};

#endif // PROXYFORM_H
