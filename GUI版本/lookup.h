#ifndef LOOKUP_H
#define LOOKUP_H

#include <QDialog>
#include <QVector>
#include "Struct.h"
namespace Ui {
class lookup;
}

class lookup : public QDialog
{
    Q_OBJECT

public:
    explicit lookup(QWidget *parent = nullptr);
    ~lookup();
    QVector<person> namelist = {};
    int vex = 0;
    double* risklist = NULL;
    path** map = NULL;
    elem** table = NULL;
    int time;
    QString iter[31]=
    {
        "北京",
        "呼和浩特",
        "银川",
        "兰州",
        "西宁",
        "乌鲁木齐",
        "拉萨",
        "昆明",
        "南宁",
        "广州",
        "福州",
        "杭州",
        "上海",
        "南京",
        "济南",
        "天津",
        "大连",
        "沈阳",
        "长春",
        "哈尔滨",
        "石家庄",
        "太原",
        "西安",
        "成都",
        "重庆",
        "贵阳",
        "长沙",
        "南昌",
        "合肥",
        "郑州",
        "武汉"
    };
    void showTable();
    void showMap();
    void showPlan();
    void showHistory();
private slots:
    void on_backbtn_clicked();
    void lookupShow();
signals:
    void back();

private:
    Ui::lookup *ui;
};

#endif // LOOKUP_H
