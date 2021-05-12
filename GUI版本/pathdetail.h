#ifndef PATHDETAIL_H
#define PATHDETAIL_H

#include <QDialog>
#include "Struct.h"
namespace Ui {
class pathdetail;
}

class pathdetail : public QDialog
{
    Q_OBJECT

public:
    explicit pathdetail(QWidget *parent,person& per);
    ~pathdetail();
    void paintEvent(QPaintEvent*);
    void showPlan();
    int index;
private slots:
    void on_backbtn_clicked();

private:
    Ui::pathdetail *ui;
    person per;
    int num;
};

#endif // PATHDETAIL_H
