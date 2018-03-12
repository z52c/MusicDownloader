#ifndef THANKS_H
#define THANKS_H

#include <QWidget>

namespace Ui {
class thanks;
}

class thanks : public QWidget
{
    Q_OBJECT

public:
    explicit thanks(QWidget *parent = 0);
    ~thanks();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::thanks *ui;
};

#endif // THANKS_H
