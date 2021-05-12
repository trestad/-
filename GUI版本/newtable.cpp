#include "newtable.h"
#include "ui_newtable.h"
#include "input_map.h"
#include <QMessageBox>
#include <QPainter>
#include <QInputDialog>
#include "Struct.h"
newtable::newtable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newtable)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon.png"));
    setWindowTitle("添加时刻表");
    setWindowFlags(Qt::Window|Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
}

newtable::~newtable()
{
    delete ui;
}

void newtable::on_pushButton_clicked()
{
    if((ui->bus->text()).toInt()>=0&&(ui->plane->text()).toInt()>=0&&(ui->train->text()).toInt()>=0)
    {
         busline = (ui->bus->text()).toInt();
         trainline = (ui->train->text()).toInt();
         planeline = (ui->plane->text()).toInt();
         int start;
         int end;
         int weight;
         int count = 0;
         while (count != busline)//输入处理汽车线路数据
         {
             input_map dlg2;
             dlg2.setGeometry(this->geometry());
             dlg2.setVexnum(vex);//帮助判断是否合法
             dlg2.setWindowTitle("创建汽车线路图");
             if (dlg2.exec() == QDialog::Accepted)
             {
                 start = dlg2.getstart();
                 end = dlg2.getend();
                 weight = dlg2.getweight();
             }
             int temp = map[start -1][end - 1].BusWeight;
             //对邻接矩阵对应上的点赋值
             if(map[start -1][end - 1].BusWeight == -1)
                 map[start - 1][end - 1].BusWeight = weight;
             else
             {
                 QMessageBox::warning(this, tr("修改数据"),tr("已存在这条线路，这样会覆盖原数据"),QMessageBox::Yes);
                  map[start - 1][end - 1].BusWeight = weight;
             }
             bool ok;
             QString str = QString("输入从%1到%2每日的汽车发车次数").arg(start).arg(end);
             int num = QInputDialog::getInt(this,"构建车次表",str,1,1,100,1,&ok,Qt::WindowMaximizeButtonHint);//最小值限制了不能为0
             if(ok == false)
             {
                 QMessageBox::warning(this, tr("Error"),tr("已取消此次输入，覆盖的数据已还原"),QMessageBox::Ok);
                 map[start - 1][end - 1].BusWeight = temp;
             }
             else                                                       //新建时刻表
             {
                 table[start - 1][end - 1].BusNext = new node;
                 table[start - 1][end - 1].BusNext->start = -1;
                 table[start - 1][end - 1].BusNext->next = NULL;
                 for (int k = 0; k < num; k++)
                 {
                     node* ptr = table[start - 1][end - 1].BusNext;
                     QString str2 = QString("请输入第%1班汽车每日发车时刻").arg(k + 1);
                     int cur_start = QInputDialog::getInt(this,"构建车次表",str2,0,0,23,1,&ok);
                     if(ok == false)
                     {
                         QMessageBox::warning(this, tr("Error"),tr("已取消此次输入，请重新输入"),QMessageBox::Ok);
                         k--;
                     }
                     else
                     {
                         node* cur = new node;
                         node* last = NULL;
                         while (ptr&& ptr->start < cur_start)                   //顺序插入
                         {
                             last = ptr;
                             ptr = ptr->next;
                         }
                         if (ptr == NULL)
                         {
                             last->next = cur;
                             cur->next = NULL;
                             cur->start = cur_start;
                         }
                         else
                         {
                             last->next = cur;
                             cur->next = ptr;
                             cur->start = cur_start;
                         }
                     }
                 }
             }
             ++count;
         }

         //处理火车，飞机线路数据
         count = 0;
         while (count != trainline)
         {
             input_map dlg2;
             dlg2.setGeometry(this->geometry());
             dlg2.setVexnum(vex);//帮助判断是否合法
             dlg2.setWindowTitle("创建火车线路图");
             if (dlg2.exec() == QDialog::Accepted)
             {
                 start = dlg2.getstart();
                 end = dlg2.getend();
                 weight = dlg2.getweight();
             }
             int temp = map[start -1][end - 1].TrainWeight;
             //对邻接矩阵对应上的点赋值
             if(map[start -1][end - 1].TrainWeight == -1)
                 map[start - 1][end - 1].TrainWeight = weight;
             else
             {
                 QMessageBox::warning(this, tr("修改数据"),tr("已存在这条线路，这样会覆盖原数据"),QMessageBox::Yes);
                 map[start - 1][end - 1].TrainWeight = weight;
             }
             bool ok;
             QString str = QString("输入从%1到%2每日的火车发车次数").arg(start).arg(end);
             int num = QInputDialog::getInt(this,"构建车次表",str,1,1,100,1,&ok);//最小值限制了不能为0
             if(ok == false)
             {
                 QMessageBox::warning(this, tr("Error"),tr("已取消此次输入，覆盖的数据已还原"),QMessageBox::Ok);
                 map[start - 1][end - 1].TrainWeight = temp;
             }
             else
             {
                 table[start - 1][end - 1].TrainNext = new node;
                 table[start - 1][end - 1].TrainNext->start = -1;
                 table[start - 1][end - 1].TrainNext->next = NULL;
                 for (int k = 0; k < num; k++)
                 {
                     node* ptr = table[start - 1][end - 1].TrainNext;
                     QString str2 = QString("请输入第%1班火车每日发车时刻").arg(k + 1);
                     int cur_start = QInputDialog::getInt(this,"构建车次表",str2,0,0,23,1);
                     node* cur = new node;
                     node* last = NULL;
                     while (ptr&& ptr->start < cur_start)
                     {
                         last = ptr;
                         ptr = ptr->next;
                     }
                     if (ptr == NULL)
                     {
                         last->next = cur;
                         cur->next = NULL;
                         cur->start = cur_start;
                     }
                     else
                     {
                         last->next = cur;
                         cur->next = ptr;
                         cur->start = cur_start;
                     }
                 }
             }
             ++count;
         }
         count = 0;
         while (count != planeline)
         {
             input_map dlg2;
             dlg2.setGeometry(this->geometry());
             dlg2.setVexnum(vex);//帮助判断是否合法
             dlg2.setWindowTitle("创建航班线路图");
             if (dlg2.exec() == QDialog::Accepted)
             {
                 start = dlg2.getstart();
                 end = dlg2.getend();
                 weight = dlg2.getweight();
             }
             int temp = map[start -1][end - 1].PlaneWeight;
             if(map[start -1][end - 1].PlaneWeight == -1)
                 map[start - 1][end - 1].PlaneWeight = weight;
             else
             {
                 QMessageBox::warning(this, tr("修改数据"),tr("已存在这条线路，这样会覆盖原数据"),QMessageBox::Yes);
                 map[start - 1][end - 1].PlaneWeight = weight;
             }
             bool ok;
             QString str = QString("输入从%1到%2每日的飞机航班次数").arg(start).arg(end);
             int num = QInputDialog::getInt(this,"构建航班表",str,1,1,100,1,&ok);//最小值限制了不能为0
             if(ok == false)
             {
                 QMessageBox::warning(this, tr("Error"),tr("已取消此次输入，覆盖的数据已还原"),QMessageBox::Ok);
                 map[start - 1][end - 1].TrainWeight = temp;
             }
             else
             {
                 table[start - 1][end - 1].PlaneNext = new node;
                 table[start - 1][end - 1].PlaneNext->start = -1;
                 table[start - 1][end - 1].PlaneNext->next = NULL;
                 for (int k = 0; k < num; k++)
                 {
                     node* ptr = table[start - 1][end - 1].PlaneNext;
                     QString str2 = QString("请输入第%1班飞机每日起飞时刻").arg(k + 1);
                     int cur_start = QInputDialog::getInt(this,"构建航班表",str2,0,0,23,1);
                     node* cur = new node;
                     node* last = NULL;
                     while (ptr&& ptr->start < cur_start)
                     {
                         last = ptr;
                         ptr = ptr->next;
                     }
                     if (ptr == NULL)
                     {
                         last->next = cur;
                         cur->next = NULL;
                         cur->start = cur_start;
                     }
                     else
                     {
                         last->next = cur;
                         cur->next = ptr;
                         cur->start = cur_start;
                     }
                 }
             }
             ++count;
         }
         emit this->back();
         accept();
    }
    else
    {
           QMessageBox::warning(this, tr("Error"),tr("添加时刻数不能是负数，添加失败"),QMessageBox::Yes);
           emit this->back();
    }
}

void newtable::paintEvent(QPaintEvent*)
{
    //背景图
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/bkg1.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);//拉伸，填满窗口
}
