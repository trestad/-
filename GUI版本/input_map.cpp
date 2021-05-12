#include "input_map.h"
#include "ui_input_map.h"
#include <QMessageBox>
#include <QPainter>

input_map::input_map(QWidget *parent) :QDialog(parent),ui(new Ui::input_map) //初始化
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icon.png"));     //图标
    setWindowFlags(Qt::Window|Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);//禁用关闭窗口
}

input_map::~input_map()
{
    delete ui;
}

bool check_edge_value(int start, int end, int weight, int vexnum)
{

    if (start<1 || end<1 || start>vexnum || end>vexnum || weight <= 0 ||start==end) //顶点从1开始编号，判断我们每次输入的的边的信息是否合法
    {
        return false;
    }
    return true;
}

void input_map::on_pushButton_clicked()         //确认按钮按下
{
    if(check_edge_value((ui->start->text()).toInt(),(ui->end->text()).toInt(),(ui->weight->text()).toInt(),vexnum))     //数据无误就返回
    {
         start = (ui->start->text()).toInt();
         end = (ui->end->text()).toInt();
         weight = (ui->weight->text()).toInt();
         accept();
    }
    else        //清空输入，重新输入
    {
           QMessageBox::warning(this, tr("Error"),tr("输入的信息不合法，请重新输入"),QMessageBox::Yes);
           ui->start->clear();
           ui->start->setFocus();
           ui->end->clear();
           ui->end->setFocus();
           ui->weight->clear();
           ui->weight->setFocus();
    }
}

void input_map::paintEvent(QPaintEvent*)
{
    //背景图
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/bkg1.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);//拉伸，填满窗口
}
