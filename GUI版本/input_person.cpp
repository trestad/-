#include "input_person.h"
#include "ui_input_person.h"
#include <QMessageBox>
#include <QPainter>
input_person::input_person(QWidget *parent) : //初始化界面
    QDialog(parent),
    ui(new Ui::input_person)
{
    ui->setupUi(this);              //调用ui文件
    ui->citylimit->setEnabled(false);       //默认禁用必经城市
    ui->timelimit->setEnabled(false);       //默认禁用时间限制
    setWindowIcon(QIcon(":/icon.png"));     //图标
    setWindowFlags(Qt::Window|Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);//禁用关闭窗口
}

input_person::~input_person()
{
    delete ui;
}

void input_person::on_confirmbtn_2_clicked()//当按下确认
{
    int startcity = ui->startcitybox_2->value();//起始城市
    int endcity = ui->destcitybox_2->value();//终点城市
    int depart = ui->departtime->value() * 24 + ui->departtimeH->value();//动身时间
    int midcity = startcity;//必经城市

    if(ui->timecheck->isChecked())//勾选了限时
    {
        kind = 2;
        limit = ui->timelimit->value();
    }
    else
    {
        kind = 1;
        limit = 999;//限时为999，即无限
    }
    if(ui->citycheck->isChecked())//是否有毕竟城市
    {
        midcity = ui->citylimit->value();
    }
    if(startcity > 0 && startcity <= vexnum && endcity > 0 && endcity <= vexnum && startcity!=endcity && midcity > 0 && midcity <= vexnum &&ui->departtimeH->value()<24)
    {//校验输入数据正确性
        start = startcity;
        end = endcity;
        departtime = depart;
        mid = midcity;
        accept();
    }
    else//输入有误，清空输入
    {
        ui->startcitybox_2->clear();
        ui->destcitybox_2->clear();
        ui->timelimit->clear();
        ui->citylimit->clear();
        ui->departtime->clear();
        ui->departtimeH->clear();
        QMessageBox::warning(this, tr("Error"),tr("输入信息不合法"),QMessageBox::Ok);
    }
}

void input_person::paintEvent(QPaintEvent*)
{
    //背景图
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/bkg1.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);//拉伸，填满窗口
}


void input_person::on_timecheck_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->timelimit->setEnabled(true);
    }
    else if(arg1 == 0)
    {
        ui->timelimit->setEnabled(false);
    }
}

void input_person::on_citycheck_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->citylimit->setEnabled(true);
    }
    else if(arg1 == 0)
    {
        ui->citylimit->setEnabled(false);
    }
}
