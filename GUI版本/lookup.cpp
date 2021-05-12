#include "lookup.h"
#include "ui_lookup.h"
#include <QPainter>
#include <QTextBrowser>
#include "Struct.h"
#include <QVector>
#include "run.h"
lookup::lookup(QWidget *parent) ://初始化界面
    QDialog(parent),
    ui(new Ui::lookup)
{
    ui->setupUi(this);
    this->setFixedSize(960,720);
    this->setWindowIcon(QIcon(":/icon.png"));
    this->setWindowTitle("COVID-19疫情环境下低风险旅行模拟系统---查询");
    setWindowFlags(Qt::Window|Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    ui->strategy->setReadOnly(true);
    ui->table->setReadOnly(true);
    ui->mapinfor->setReadOnly(true);
    ui->status->setReadOnly(true);
    connect(parent, SIGNAL(lookupsig()), this, SLOT(lookupShow()));
}

lookup::~lookup()
{
    delete ui;
}
void lookup::lookupShow()//显示四大窗口
{
    ui->table->clear();
    ui->mapinfor->clear();
    ui->strategy->clear();
    ui->status->clear();
    showTable();
    showMap();
    showPlan();
    showHistory();
}
void lookup::on_backbtn_clicked()//点击确认键返回运行界面
{
    emit this->back();
    accept();
}

void lookup::showTable()//显示时刻表
{
    ui->table->insertPlainText("\n--------------------汽车时刻表--------------------\n");
    for (int i = 0; i < this->vex; i++)
    {
        for (int j = 0; j < this->vex; j++)
        {
            if (this->table[i][j].BusNext != NULL)
            {
                ui->table->insertPlainText("\n" + iter[i] + "到" + iter[j] + "的汽车每日发车时刻：\n");
                node* show = (this->table[i][j].BusNext)->next;
                while (show!=NULL)
                {
                    ui->table->insertPlainText(QString::number(show->start)+ ":00 ");
                    show = show->next;
                }
            }
        }
    }
    ui->table->insertPlainText("\n---------------------火车时刻表---------------------\n");
    for (int i = 0; i < this->vex; i++)
    {
        for (int j = 0; j < this->vex; j++)
        {
            if (this->table[i][j].TrainNext != NULL)
            {
                ui->table->insertPlainText("\n" + iter[i] + "到" + iter[j] + "的火车每日发车时刻：\n");
                node* show = (this->table[i][j].TrainNext)->next;
                while (show!=NULL)
                {
                    ui->table->insertPlainText(QString::number(show->start)+ ":00 ");
                    show = show->next;
                }
            }
        }
    }
    ui->table->insertPlainText("\n---------------------飞机时刻表---------------------\n");
    for (int i = 0; i < this->vex; i++)
    {
        for (int j = 0; j < this->vex; j++)
        {
            if (this->table[i][j].PlaneNext != NULL)
            {
                ui->table->insertPlainText("\n" + iter[i] + "到" + iter[j] + "的航班每日起飞时刻：\n");
                node* show = (this->table[i][j].PlaneNext)->next;
                while (show!=NULL)
                {
                    ui->table->insertPlainText(QString::number(show->start)+ ":00 ");
                    show = show->next;
                }
            }
        }
    }
}
void lookup::showMap()//显示地图信息，邻接矩阵被我注释掉了，感觉图形界面足以描述清楚必要信息了。
{
    ui->mapinfor->insertPlainText("-------------------各城市风险值---------------------\n");
    for (int i = 0; i < this->vex; i++)
    {
        ui->mapinfor->insertPlainText(iter[i] + "的风险值为" + QString::number(this->risklist[i]) + "\n");
    }

    /*int count_row = 0;
    int count_col = 0;
    ui->mapinfor->insertPlainText("-------------------汽车线路邻接矩阵-------------------\n");
    while (count_row != this->vex)
    {
        count_col = 0;
        while (count_col != this->vex)
        {
            if (map[count_row][count_col].BusWeight == -1)
                ui->mapinfor->insertPlainText(" ∞ ");
            else
                ui->mapinfor->insertPlainText(" " + QString::number(map[count_row][count_col].BusWeight) + " ");
            ++count_col;
        }
        ui->mapinfor->insertPlainText("\n");
        ++count_row;
    }
    count_row = 0;
    ui->mapinfor->insertPlainText("-------------------火车线路邻接矩阵-------------------\n");
    while (count_row != this->vex)
    {
        count_col = 0;
        while (count_col != this->vex)
        {
            if (map[count_row][count_col].TrainWeight == -1)
                ui->mapinfor->insertPlainText(" ∞ ");
            else
                ui->mapinfor->insertPlainText(" " + QString::number(map[count_row][count_col].TrainWeight) + " ");
            ++count_col;
        }
        ui->mapinfor->insertPlainText("\n");
        ++count_row;
    }
    count_row = 0;
    ui->mapinfor->insertPlainText("-------------------飞机航班邻接矩阵-------------------\n");
    while (count_row != this->vex)
    {
        count_col = 0;
        while (count_col != this->vex)
        {
            if (map[count_row][count_col].PlaneWeight == -1)
                ui->mapinfor->insertPlainText(" ∞ ");
            else
                ui->mapinfor->insertPlainText(" " + QString::number(map[count_row][count_col].PlaneWeight) + " ");
            ++count_col;
        }
        ui->mapinfor->insertPlainText("\n");
        ++count_row;
    }*/

}
void lookup::showPlan()     //显示各旅客计划
{
    QString str;
    for(int i =0;i<this->namelist.size();i++)
    {
        ui->strategy->insertPlainText("第" + QString::number(i+1) + "名旅客选择的路径：\n");
        for (auto it = namelist[i].chosenplan.begin(); it != namelist[i].chosenplan.end(); it++)
        {
            if ((*it).transport == 'b')
            {
                str = "汽车";
            }
            if ((*it).transport == 't')
            {
                str = "火车";
            }
            if ((*it).transport == 'p')
            {
                str = "飞机";
            }
            ui->strategy->insertPlainText("在" + iter[(*it).curcity - 1] + "滞留" + QString::number((*it).staytime)
                                     + "小时后搭乘" + str + "前往" + iter[(*it).nextcity - 1]
                                     + "，到达时间为第" + QString::number((*it).arrivetime / 24)
                                     + "天" + QString::number((*it).arrivetime % 24)  + "点，此时总风险为" + QString::number((*it).risk) + "\n");
        }
    }
}
void lookup::showHistory()      //显示历史事件
{

    for(int t = 0 ;t<=time;t++)
    {
        QString outputtime;
        outputtime = "\n当前时间：第" + QString::number(t / 24) + "天" + QString::number(t % 24) + "点:";
        ui->status->insertPlainText(outputtime);
        int cnt = 1;
        for (auto i = namelist.begin(); i != namelist.end(); i++)
        {
            QString str,status,index;
            index = "\n第" + QString::number(cnt) + "号旅客状态：\n";
            ui->status->insertPlainText(index);
            int flag;
            int start_to_move = (*i).departTime;

            for (auto it = (*i).chosenplan.begin(); it != (*i).chosenplan.end(); it++)
            {
                flag = 1;
                int arrivetime = (*it).arrivetime;
                int depart = (*it).arrivetime - (*it).nextcost;
                int stay = (*it).arrivetime - (*it).staytime - (*it).nextcost;//前一个城市到达时间，即开始滞留的时间
                double risk = 0;
                int rate = 0;
                if ((*it).transport == 'b')
                {
                    str = "汽车";
                    rate = 2;
                }
                if ((*it).transport == 't')
                {
                    str = "火车";
                    rate = 5;
                }
                if ((*it).transport == 'p')
                {
                    str = "飞机";
                    rate = 9;
                }
                if (t > arrivetime)
                {
                    flag = 0;
                    continue;
                }
                else if (t < start_to_move)
                {
                    risk = 0;
                    status = "还未出发\n当前累计风险值为" + QString::number(risk) + "\n";
                    ui->status->insertPlainText(status);
                    break;
                }
                else if (t == arrivetime)
                {
                    risk = (*it).risk;
                    status = "刚好到达城市" + QString::number((*it).nextcity) + "\n当前累计风险值为" + QString::number(risk) + "\n";
                    ui->status->insertPlainText(status);
                    break;
                }
                else if (t >= depart && t < arrivetime)
                {

                    risk = (*it).risk - risklist[(*it).curcity - 1] * rate * (arrivetime - t);
                    status = "从" + iter[(*it).curcity-1] + "乘坐" + str + "前往" + iter[(*it).nextcity-1] + "\n当前累计风险值为" + QString::number(risk, 'f', 1) + "\n";//防止第0秒出发限时无穷小
                    ui->status->insertPlainText(status);
                    break;
                }
                else if (t < depart && t >= stay)
                {

                    risk = (*it).risk - risklist[(*it).curcity - 1] * ((arrivetime - t - (*it).nextcost) + (*it).nextcost * rate);
                    status = "在" + iter[(*it).curcity - 1] + "滞留中\n当前累计风险值为" + QString::number(risk, 'f', 1) + "\n";
                    ui->status->insertPlainText(status);
                    break;
                }
            }
            if (flag == 0)
            {
                status = "已经到达目的地\n";
                ui->status->insertPlainText(status);
            }
            cnt++;
        }
    }
}
