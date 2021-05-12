#ifndef CHANGE_PATH_H
#define CHANGE_PATH_H

#include <QDialog>

namespace Ui {
class change_path;
}

class change_path : public QDialog
{
    Q_OBJECT

public:
    explicit change_path(QWidget *parent,int start);
    ~change_path();
    int getend()
    {
        return end;
    }
    int getkind()
    {
        return kind;
    }
    void setvex(int vex)
    {
        vexnum = vex;
    }
    int getdepart()
    {
        return departtime;
    }
    int getlimit()
    {
        return limit;
    }
    int getmid()
    {
        return mid;
    }
    void paintEvent(QPaintEvent*);
private slots:
    void on_timecheck_stateChanged(int arg1);

    void on_citycheck_stateChanged(int arg1);

    void on_pushButton_clicked();

private:
    Ui::change_path *ui;
    int kind;
    int start;
    int end;
    int limit;
    int mid;
    int vexnum;
    int departtime;
};

#endif // CHANGE_PATH_H
