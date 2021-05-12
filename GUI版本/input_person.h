#ifndef INPUTPERINFOR_H
#define INPUTPERINFOR_H

#include <QDialog>

namespace Ui {
class input_person;
}

class input_person : public QDialog
{
    Q_OBJECT

public:
    explicit input_person(QWidget *parent = nullptr);
    ~input_person();
    void paintEvent(QPaintEvent*);
    int getstart()
    {
        return start;
    }
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
private slots:
    void on_confirmbtn_2_clicked();
    void on_timecheck_stateChanged(int arg1);

    void on_citycheck_stateChanged(int arg1);

private:
    Ui::input_person *ui;
    int kind;
    int start;
    int end;
    int limit;
    int mid;
    int vexnum;
    int departtime;
};

#endif // INPUTPERINFOR_H
