#ifndef NEWCITY_H
#define NEWCITY_H

#include <QDialog>
#include"Struct.h"
namespace Ui {
class newtable;
}

class newtable : public QDialog
{
    Q_OBJECT

public:
    explicit newtable(QWidget *parent = NULL);
    ~newtable();
    void paintEvent(QPaintEvent*);
    int busline;
    int trainline;
    int planeline;
    int vex;
    elem** table;
    path** map;
signals:

    void back();
private slots:
    void on_pushButton_clicked();

private:
    Ui::newtable *ui;
};

#endif // NEWCITY_H
