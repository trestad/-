#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <QVector>
#include "Struct.h"
namespace Ui {
class input_data;
}

class input_data : public QDialog
{
    Q_OBJECT

public:
    explicit input_data(QWidget *parent = nullptr);

    ~input_data();
    void paintEvent(QPaintEvent*);
    int vexnum;

    int edgeB;

    int edgeT;

    int edgeP;

    int PersonNum;

    double* risklist;

    elem** table;

    path** map;

    QVector<person> namelist;
signals:

    void back_to_main();


private slots:

    void on_confirmbtn_clicked();

private:
    Ui::input_data *ui;
    QString iter[31]=
    {
        "北京",//1
        "呼和浩特",//2
        "银川",//3
        "兰州",//4
        "西宁",//5
        "乌鲁木齐",//6
        "拉萨",//7
        "昆明",//8
        "南宁",//9
        "广州",//10
        "福州",//11
        "杭州",//12
        "上海",//13
        "南京",//14
        "济南",//15
        "天津",//16
        "大连",//17
        "沈阳",//18
        "长春",//19
        "哈尔滨",//20
        "石家庄",//21
        "太原",//22
        "西安",//23
        "成都",//24
        "重庆",//25
        "贵阳",//26
        "长沙",//27
        "南昌",//28
        "合肥",//29
        "郑州",//30
        "武汉"//31
    };
    //地图信息
    void creategraph();
    void createperson();
};

#endif // INPUTDIALOG_H
