#include "pathdetail.h"
#include "ui_pathdetail.h"
#include "Struct.h"
#include <QMessageBox>
#include <QPainter>
pathdetail::pathdetail(QWidget *parent,person& per) :
    QDialog(parent),
    ui(new Ui::pathdetail)
{
    ui->setupUi(this);
    this->per = per;
    setWindowTitle("规划出的最优路径");
    //设置图标
    setWindowIcon(QIcon(":/icon.png"));
    setWindowFlags(Qt::Window|Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    showPlan();
}

pathdetail::~pathdetail()
{
    delete ui;
}
void pathdetail::showPlan()
{
    //序号转为名字
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
    QString str;
    if(per.bestplan.size() == 1)//如果只有一条最优解，直接输出
    {
        ui->textBrowser->insertPlainText("有一条最优路径：\n\n");
        ui->label->hide();
        ui->lineEdit->hide();
    }
    else
        ui->textBrowser->insertPlainText("有多条风险相等的路径，请选择：\n\n");


    //把最优解全部输出给用户
    num = 1;
    for (auto i = per.bestplan.begin(); i != per.bestplan.end(); i++)
    {
        ui->textBrowser->insertPlainText("路径" + QString::number(num) + ": \n");
        for (auto it = i->begin(); it != i->end(); it++)
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
            ui->textBrowser->insertPlainText("在" + iter[(*it).curcity - 1] + "滞留" + QString::number((*it).staytime)
                                     + "小时后搭乘" + str + "前往" + iter[(*it).nextcity - 1]
                                     + "，到达时间为第" + QString::number((*it).arrivetime / 24)
                                     + "天" + QString::number((*it).arrivetime % 24)  + "点，此时总风险为" + QString::number((*it).risk) + "\n\n");
        }
        num++;
    }
}


void pathdetail::on_backbtn_clicked()//用户点击确认，选择一个最优解
{
    if(per.bestplan.size() == 1)//只有一个最优解直接返回
    {
        accept();
        return;
    }
    if((ui->lineEdit->text()).toInt()<num)//有多个的话，返回输入序号
    {
        this->index = (ui->lineEdit->text()).toInt() - 1;
        accept();
        return;
    }
    else
    {
        QMessageBox::warning(this, tr("Error"),tr("输入的数值不合法，请重新输入"),QMessageBox::Ok);
        ui->lineEdit->clear();
        ui->lineEdit->setFocus();
    }
}

void pathdetail::paintEvent(QPaintEvent*)
{
    //背景图
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/bkg1.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);//拉伸，填满窗口
}
