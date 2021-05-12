#ifndef INPUTDIALOG2_H
#define INPUTDIALOG2_H

#include <QDialog>

namespace Ui {
class input_map;
}

class input_map : public QDialog
{
    Q_OBJECT

public:
    explicit input_map(QWidget *parent = nullptr);
    ~input_map();
    void paintEvent(QPaintEvent*);
    void setVexnum(int vex)
    {
        vexnum = vex;
    }
    int getstart()
    {
        return start;
    }
    int getend()
    {
        return end;
    }
    int getweight()
    {
        return weight;
    }
private slots:
    void on_pushButton_clicked();

private:
    Ui::input_map *ui;
    int start;
    int end;
    int weight;
    int vexnum;

};

#endif // INPUTDIALOG2_H
