#include "input_data.h"
#include "ui_input_data.h"
#include "mypushbutton.h"
#include "input_map.h"
#include "input_person.h"
#include "Struct.h"
#include <QMessageBox>
#include <QPainter>
#include <QInputDialog>
#include <QSequentialAnimationGroup>
#include "mainwindow.h"
#include "pathdetail.h"
#include "Struct.h"
input_data::input_data(QWidget *parent) ://初始化界面
    QDialog(parent),
    ui(new Ui::input_data)
{
    ui->setupUi(this);
    setWindowTitle("创建地图");
    //设置图标
    setWindowIcon(QIcon(":/icon.png"));
}

input_data::~input_data()
{
    delete ui;
}

bool check(int Vexnum, int edgeB, int edgeT, int edgeP)//输入图前的检验
{
    if (Vexnum <= 31&&(Vexnum * (Vexnum - 1)) >= edgeB && Vexnum * (Vexnum - 1) >= edgeT && Vexnum * (Vexnum - 1) >= edgeP && Vexnum > 1 && edgeB>=0 && edgeT>=0 && edgeP>=0)
        return true;            //城市数小于31，两城市的一种交通工具直接不得重复输入边，各项值不得小于0
    else
        return false;
}

void input_data::on_confirmbtn_clicked()                //点击了确认键后
{
    if(check((ui->citylineEdit->text()).toInt(),(ui->edgeBlineEdit->text()).toInt(),(ui->edgeTlineEdit->text()).toInt(),(ui->edgePlineEdit->text()).toInt()))
    {
         vexnum = (ui->citylineEdit->text()).toInt();               //赋值
         edgeB = (ui->edgeBlineEdit->text()).toInt();
         edgeT = (ui->edgeTlineEdit->text()).toInt();
         edgeP = (ui->edgePlineEdit->text()).toInt();
         this->risklist = new double[this->vexnum];                 //开辟城市风险值数组空间
         for(int i = 0;i<this->vexnum;i++)
         {
             bool ok;
             QString str = "输入" + iter[i] + "风险指数: ";           //输入各城市风险值
             double b = QInputDialog::getDouble(this,"风险",str,0.2,0.2,0.9,1,&ok,Qt::WindowMaximizeButtonHint);
             {
                 this->risklist[i] = b;
             }
             if(ok == false)                                            //取消输入的提示
             {
                 QMessageBox::warning(this, tr("Error"),tr("已取消此次输入，请重新输入"),QMessageBox::Ok);
                 i--;
             }
         }

         //开辟地图与时刻表空间
         map = new path * [this->vexnum];
         table = new elem * [this->vexnum];

         for (int i = 0; i < this->vexnum; i++)
         {
             table[i] = new elem[this->vexnum];
             map[i] = new path[this->vexnum];

             for (int k = 0; k < this->vexnum; k++)
             {
                 map[i][k].BusWeight = -1;
                 map[i][k].TrainWeight = -1;
                 map[i][k].PlaneWeight = -1;

                 table[i][k].BusNext = NULL;
                 table[i][k].PlaneNext = NULL;
                 table[i][k].TrainNext = NULL;
             }
         }

         creategraph();     //构造时刻表与地图的邻接矩阵

         while(1)           //输入最初的旅客数量
         {
            bool ok;
            PersonNum = QInputDialog::getInt(this,"开始创建初始旅客信息","请输入初始旅客数",0,0,10,1,&ok,Qt::WindowMaximizeButtonHint);//最小值限制了不能为0,最多10个旅客
            if(ok == true)
                break;
            else
            {
                QMessageBox::warning(this, tr("Error"),tr("取消了输入，请重新输入"),QMessageBox::Ok);
            }
         }

         createperson();    //创建旅客

         MainWindow *ptr = (MainWindow*)parentWidget();     //把输入的信息返回给主函数，以便传入运行界面
         ptr->namelist = this->namelist;
         ptr->map = this->map;
         ptr->vex = this->vexnum;
         ptr->table = this->table;
         ptr->risklist = this->risklist;
         QMessageBox::about(this, tr("返回到主界面"),tr("信息输入完成"));
         emit this->back_to_main();                         //发出信号，返回主函数

    }
    else            //输入不合法，清空输入，重新输入
    {
           QMessageBox::warning(this, tr("Error"),tr("输入的数值不合法，请重新输入"),QMessageBox::Ok);
           ui->citylineEdit->clear();
           ui->citylineEdit->setFocus();
           ui->edgeBlineEdit->clear();
           ui->edgeBlineEdit->setFocus();
           ui->edgeTlineEdit->clear();
           ui->edgeTlineEdit->setFocus();
           ui->edgePlineEdit->clear();
           ui->edgePlineEdit->setFocus();
    }
}

void input_data::createperson()             //创建旅客信息
{
    int start;          //起始城市
    int mid;            //必经城市
    int end;            //终点城市
    int kind;           //策略
    int limit;          //时间限制
    int depart;         //出发时间
    for(int i =0 ; i<PersonNum;i++)   //依次输入各旅客的信息
    {
        input_person dlg(this);       //进入输入旅客信息界面
        dlg.setvex(vexnum);
        dlg.setGeometry(this->geometry());
        QString str = QString("请输入第%1个旅客的信息").arg(i + 1);
        dlg.setWindowTitle(str);
        if (dlg.exec() == QDialog::Accepted)        //信息输入无误，返回到此页面
        {
            start = dlg.getstart();                 //赋值
            end = dlg.getend();
            kind = dlg.getkind();
            depart = dlg.getdepart();
            limit = dlg.getlimit();
            mid = dlg.getmid();

            person per(PersonNum,start,mid,end,kind,depart,limit,vexnum,risklist,map,table);        //创建旅客

            if(per.pathexist==0)       //如果没有满足需求的规划
            {
                QMessageBox::critical(this, tr("Error"),tr("不存在这样的路径，此次输入作废"),QMessageBox::Ok);
                i = i - 1;              //这次输入不计数
            }
            else
            {
                namelist.push_back(per);        //加入到旅客名单中
                pathdetail* showtemp = new pathdetail(this,per);        //调用显示路径规划详情界面
                showtemp->show();                                       //显示
                this->hide();
                showtemp->setGeometry(this->geometry());
                if (showtemp->exec() == QDialog::Accepted)              //在路径规划详情界面点击确认后，返回
                {}
                if(per.chosenplan.size()==0)                            //如果系统没有默认选择路径，交由用户来选择第index号路径
                {
                    namelist.back().chosenplan = per.bestplan[showtemp->index];
                }
                per.bestplan.clear();                                   //清空bestplan节约空间。
            }
        }
    }
}

void input_data::creategraph()                  //创建地图信息
{
    int start;          //起始城市
    int end;            //终止城市
    int weight;         //路径权值
    int count = 0;      //计数
    while (count != edgeB)      //汽车的路线输入
    {
        input_map dlg(this);        //调用输入地图窗口
        dlg.setGeometry(this->geometry());
        dlg.setVexnum(vexnum);
        dlg.setWindowTitle("创建汽车线路图");
        if (dlg.exec() == QDialog::Accepted)        //数据无误，返回，赋值
        {
            start = dlg.getstart();
            end = dlg.getend();
            weight = dlg.getweight();
        }
        //对邻接矩阵对应上的点赋值
        if(map[start -1][end - 1].BusWeight == -1)
            map[start - 1][end - 1].BusWeight = weight;
        else
        {
            QMessageBox::critical(this, tr("Error"),tr("这条路线已经输入过了！"),QMessageBox::Yes);
            --count;
        }
        ++count;
    }
    for (int i = 0; i < vexnum; i++)
    {
        for (int j = 0; j < vexnum; j++)
        {
            if (map[i][j].BusWeight != -1)
            {
                bool ok;
                QString str = "输入从" + iter[i] + "到" + iter[j] + "每日的汽车发车次数";
                int num = QInputDialog::getInt(this,"构建车次表",str,1,1,100,1,&ok,Qt::WindowMaximizeButtonHint);//最小值限制了不能为0
                if(ok == false)
                {
                    QMessageBox::warning(this, tr("Error"),tr("已取消此次输入，请重新输入"),QMessageBox::Ok);
                    j--;
                }
                else
                {
                    table[i][j].BusNext = new node;             //开辟节点空间
                    table[i][j].BusNext->start = -1;
                    table[i][j].BusNext->next = NULL;
                    for (int k = 0; k < num; k++)
                    {
                        node* ptr = table[i][j].BusNext;
                        QString str2 = QString("请输入第%1班汽车每日首发时刻").arg(k + 1);
                        int cur_start = QInputDialog::getInt(this,"构建车次表",str2,0,0,23,1,&ok,Qt::WindowMaximizeButtonHint);
                        if(ok == true)
                        {
                            node* cur = new node;               //时刻表顺序插入算法
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
                        else
                        {
                            QMessageBox::warning(this, tr("Error"),tr("已取消此次输入，请重新输入"),QMessageBox::Ok);
                            k--;
                        }
                    }
                }
            }
        }
    }







    //下面分别是火车路径和飞机路径输入
    count = 0;
    while (count != edgeT)
    {
        input_map dlg(this);
        dlg.setGeometry(this->geometry());
        dlg.setVexnum(vexnum);
        dlg.setWindowTitle("创建火车线路图");
        if (dlg.exec() == QDialog::Accepted)
        {
            start = dlg.getstart();
            end = dlg.getend();
            weight = dlg.getweight();
        }
        if(map[start -1][end - 1].TrainWeight == -1)
            map[start - 1][end - 1].TrainWeight = weight;
        else
        {
            QMessageBox::critical(this, tr("Error"),tr("这条路线已经输入过了！"),QMessageBox::Yes);
            --count;
        }
        ++count;
    }
    for (int i = 0; i < vexnum; i++)
    {
        for (int j = 0; j < vexnum; j++)
        {
            if (map[i][j].TrainWeight != -1)
            {
                bool ok;
                QString str = QString("输入从" + iter[i] + "到" + iter[j] + "每日火车发车次数").arg(i + 1).arg(j + 1);
                int num = QInputDialog::getInt(this,"构建车次表",str,1,1,30,1,&ok,Qt::WindowMaximizeButtonHint);//最小值限制了不能为0
                if(ok == false)
                {
                    QMessageBox::warning(this, tr("Error"),tr("已取消此次输入，请重新输入"),QMessageBox::Ok);
                    j--;
                }
                else
                {
                    table[i][j].TrainNext = new node;
                    table[i][j].TrainNext->start = -1;
                    table[i][j].TrainNext->next = NULL;
                    for (int k = 0; k < num; k++)
                    {
                        node* ptr = table[i][j].TrainNext;
                        QString str2 = QString("请输入第%1班火车每日首发时刻").arg(k + 1);
                        int cur_start = QInputDialog::getInt(this,"构建车次表",str2,0,0,23,1,&ok,Qt::WindowMaximizeButtonHint);
                        if(ok == true)
                        {
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
                        else
                        {
                            QMessageBox::warning(this, tr("Error"),tr("已取消此次输入，请重新输入"),QMessageBox::Ok);
                            k--;
                        }
                    }
                }
            }
        }
    }

    //飞机
    count = 0;
    while (count != edgeP)
    {
        input_map dlg(this);
        dlg.setGeometry(this->geometry());
        dlg.setVexnum(vexnum);
        dlg.setWindowTitle("创建飞机线路图");
        if (dlg.exec() == QDialog::Accepted)
        {
            start = dlg.getstart();
            end = dlg.getend();
            weight = dlg.getweight();
        }
        if(map[start -1][end - 1].PlaneWeight == -1)
            map[start - 1][end - 1].PlaneWeight = weight;
        else
        {
            QMessageBox::critical(this, tr("Error"),tr("这条路线已经输入过了！"),QMessageBox::Yes);
            --count;
        }
        ++count;
    }
    for (int i = 0; i < vexnum; i++)
    {
        for (int j = 0; j < vexnum; j++)
        {
            if (map[i][j].PlaneWeight != -1)
            {
                bool ok;
                QString str = QString("输入从" + iter[i] + "到" + iter[j] + "每日的航班次数").arg(i + 1).arg(j + 1);
                int num = QInputDialog::getInt(this,"构建航班表",str,1,1,10,1,&ok,Qt::WindowMaximizeButtonHint);//最小值限制了不能为0
                if(ok == false)
                {
                    QMessageBox::warning(this, tr("Error"),tr("已取消此次输入，请重新输入"),QMessageBox::Ok);
                    j--;
                }
                else
                {
                    table[i][j].PlaneNext = new node;
                    table[i][j].PlaneNext->start = -1;
                    table[i][j].PlaneNext->next = NULL;
                    for (int k = 0; k < num; k++)
                    {
                        node* ptr = table[i][j].PlaneNext;
                        QString str2 = QString("请输入第%1班飞机每日首发时刻").arg(k + 1);
                        int cur_start = QInputDialog::getInt(this,"构建航班表",str2,0,0,23,1,&ok,Qt::WindowMaximizeButtonHint);
                        if(ok == true)
                        {
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
                        else
                        {
                            QMessageBox::warning(this, tr("Error"),tr("已取消此次输入，请重新输入"),QMessageBox::Ok);
                            k--;
                        }
                    }
                }
            }
        }
    }


}

void input_data::paintEvent(QPaintEvent*)
{
    //背景图
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/bkg1.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);//拉伸，填满窗口
}
