#ifndef RUN_H
#define RUN_H
#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QTextBrowser>
#include <QElapsedTimer>
#include <QTime>
#include <QTextStream>
#include <QDir>
#include <QVector>
#include <QMessageBox>
#include <QApplication>
#include "Struct.h"
#include <QParallelAnimationGroup>
class run : public QWidget
{
    Q_OBJECT
    int m_timerId;
    qint64 m_accumulator;
    QLabel *m_label;
    QElapsedTimer m_timer;
    Q_SLOT void on_restart_clicked()
    {
        m_accumulator = 0;
        m_timer.restart();
        if (m_timerId == -1)
            m_timerId = startTimer(50);
    }
    Q_SLOT void on_pause_clicked()
    {
        if (m_timer.isValid())
        {
            m_accumulator += m_timer.elapsed();
            m_timer.invalidate();
        }
        else
        {
            m_timer.restart();
            m_timerId = startTimer(50);
        }
    }

public:
    run(QWidget *parent,QVector<person> &namelist,double* risklist,elem** table,path** map,int vex);
    QVector<person> namelist;
    void paintEvent(QPaintEvent*);
    void timerEvent(QTimerEvent * ev);
    int vex;
    void output_log(QString str)
    {
        QDateTime da_time;
        QString time_str = da_time.currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
        QFile file(QDir::currentPath()+"/log.txt");
        if(!file.open(QFile::WriteOnly | QFile::Text|QIODevice::Append))
        {
            return;
        }
        QTextStream stream(&file);
        stream<<time_str+":"+str<<"\n";
        file.close();
    }
private:
    double* risklist;
    QLabel* timelabel = NULL;
    QLabel* daylabel = NULL;
    path** map;
    elem** table;
    QTextBrowser *log;
    QFont font;
    int day = 0;
    int hour =-1;
    bool isChanging = false;
    QParallelAnimationGroup * paraGroup;
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
    int build[31][2]=
    {
        {1074,402},//北京
        {959,388},//呼和浩特
        {831,434},//银川
        {777,499},//兰州
        {720,486},//西宁
        {379,315},//乌鲁木齐
        {469,645},//拉萨
        {740,755},//昆明
        {888,796},//南宁
        {1000,790},//广州
        {1142,730},//福州
        {1169,634},//杭州
        {1193,610},//上海
        {1138,594},//南京
        {1102,479},//济南
        {1098,420},//天津
        {1195,425},//大连
        {1242,362},//沈阳
        {1303,306},//长春
        {1325,243},//哈尔滨
        {1032,446},//石家庄
        {980,448},//太原
        {894,530},//西安
        {775,629},//成都
        {840,650},//重庆
        {844,721},//贵阳
        {986,678},//长沙
        {1059,666},//南昌
        {1100,598},//合肥
        {1012,532},//郑州
        {1025,626}//武汉
    };
signals:
    void lookupsig();

};

#endif // RUN_H
