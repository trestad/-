#include "change_path.h"
#include "ui_change_path.h"
#include <QMessageBox>
#include <QPainter>
change_path::change_path(QWidget *parent,int stt) ://修改路径构造函数
    QDialog(parent),
    ui(new Ui::change_path)
{
    this->start = stt;                                                          //默认起始城市
    ui->setupUi(this);                                                          //连接ui文件
    ui->citylimit->setEnabled(false);                                           //默认不勾选必经城市
    ui->timelimit->setEnabled(false);                                           //也不勾选时间限制
    setWindowIcon(QIcon(":/icon.png"));                                         //设置窗口图标
    //setWindowFlags(Qt::Window|Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);     //禁用关闭窗口按钮
    ui->text->setText("当前所在城市：" + QString::number(start));                  //设置显示提示文字
    ui->text->setGeometry(0,0,640,30);                                           //设置大小
}

change_path::~change_path()                                                       //析构函数
{
    delete ui;
}

void change_path::on_timecheck_stateChanged(int arg1)                               //槽函数，勾选时间限制按键的状态变化
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

void change_path::on_citycheck_stateChanged(int arg1)                               //槽函数，勾选必经城市按键的状态变化
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

void change_path::on_pushButton_clicked()                                              //槽函数，点击确定按钮返回到运行界面
{
    int endcity = ui->end->value();
    int depart = ui->departday->value() * 24 + ui->departhour->value();
    int midcity = start;

    if(ui->timecheck->isChecked())                                                     //勾选了时间限制
    {
        kind = 2;
        limit = ui->timelimit->value();
    }
    else
    {
        kind = 1;
        limit = 999;
    }
    if(ui->citycheck->isChecked())                                                      //勾选了必经城市
    {
        midcity = ui->citylimit->value();
    }
    if(endcity > 0 && endcity <= vexnum && start!=endcity && midcity > 0 && midcity <= vexnum &&ui->departhour->value()<24)
    {
        end = endcity;
        departtime = depart;
        mid = midcity;
        accept();                    //取消运行界面的阻塞
    }
    else                                                                                //输入有误，清空输入
    {
        ui->end->clear();
        ui->timelimit->clear();
        ui->citylimit->clear();
        ui->departday->clear();
        ui->departhour->clear();
        QMessageBox::warning(this, tr("Error"),tr("输入信息不合法"),QMessageBox::Ok);
    }
}
void change_path::paintEvent(QPaintEvent*)
{
    //背景图
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/bkg1.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);//拉伸，填满窗口
}
