#include <QLabel>
#include <QTimer>
#include <QPainter>
#include <QMessageBox>
#include <QTextBrowser>
#include <QInputDialog>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include "run.h"
#include "lookup.h"
#include "newtable.h"
#include "input_data.h"
#include "pathdetail.h"
#include "input_person.h"
#include "mypushbutton.h"
#include "change_path.h"

run::run(QWidget *parent,QVector<person>& namelist,double* risklist,elem** table,path** map,int vex) : QWidget(parent),m_timerId(-1)
{
    output_log("进入运行");

    //场景初始化
    this->setWindowIcon(QIcon(":/icon.png"));
    this->setWindowTitle("COVID-19疫情环境下低风险旅行模拟系统---模拟中");
    this->setFixedSize(1533,900);
    //数据初始化
    this->risklist = risklist;
    this->table = table;
    this->map = map;
    this->vex = vex;
    this->namelist = namelist;
    //字体初始化
    font.setFamily("Microsoft YaHei");
    font.setPointSize(10);

    //计时器初始化
    on_restart_clicked();
    m_label = new QLabel(this);
    m_label->setGeometry(10,10,400,80);
    QFont qfont;
    qfont.setFamily("Microsoft YaHei");
    qfont.setPointSize(15);
    qfont.setBold(true);
    m_label->setFont(qfont);
    m_label->setStyleSheet("color:red;");
    m_label->show();

    //界面日志设定
    QLabel * illust_status = new QLabel;
    illust_status->setParent(this);
    illust_status->setFont(font);
    illust_status->setText("状态日志");
    illust_status->setGeometry(5,545,80,20);

    this->log = new QTextBrowser;
    this->log->setParent(this);
    this->log->setFixedSize(250,300);
    this->log->move(7,570);
    this->log->setReadOnly(true);
    this->log->setEnabled(false);
    this->log->show();


    //初始化并行动画组
    QString str = ":/bus.png";
    QLabel * lab = new QLabel(str);
    paraGroup = new QParallelAnimationGroup;
    QPropertyAnimation * stuff = new QPropertyAnimation(lab,"geometry");
    stuff->setStartValue(QRect(0,0,0,0));
    stuff->setEndValue(QRect(1,1,1,1));
    stuff->setDuration(999*10000);
    paraGroup->addAnimation(stuff);

    for (auto i = this->namelist.begin(); i != this->namelist.end(); i++)
    {
        i->pic = new QLabel(this);                                  //旅客维护的label，用来显示图片
        i->seqGroup = new QSequentialAnimationGroup(this);              //每个旅客内部维护一个seq group
        connect(i->seqGroup,&QPropertyAnimation::finished,[=]()
        {
            delete i->pic;
         });
        for (auto it = (*i).chosenplan.begin(); it != (*i).chosenplan.end(); it++)
        {
            QString transp;
            if ((*it).transport == 'b')                                 //旅客下一阶段乘坐的交通工具图片
            {
                transp = ":/bus.png";
            }
            if ((*it).transport == 't')
            {
                transp = ":/train.png";
            }
            if ((*it).transport == 'p')
            {
                transp = ":/plane.png";
            }
            int duration = (*it).nextcost;                              //旅行用时
            QPropertyAnimation *animation = new QPropertyAnimation(i->pic, "geometry");//两城市之间的一个动画
            i->seqGroup->addPause(10000 * (*it).staytime);               //要是旅客到了一个城市，还需要等车，给seq加一段暂停时间
            animation->setStartValue(QRect(build[(*it).curcity - 1][0]-25,build[(*it).curcity - 1][1]-25,50,50));//在这一阶段的旅行中，起始城市坐标
            animation->setEndValue(QRect(build[(*it).nextcity - 1][0]-25,build[(*it).nextcity - 1][1]-25,50,50));//终点城市坐标
            animation->setDuration(duration * 10000);
            connect(animation,&QPropertyAnimation::stateChanged,[=]()
            {
                i->pic->setPixmap(transp);
             });
            i->seqGroup->addAnimation(animation);                       //把这一小段旅程动画加到这个旅客的整个旅程的动画中去
        }
        this->paraGroup->addAnimation(i->seqGroup);                     //所有的旅客一起组成一个并行的组，方便一起暂停
    }
    paraGroup->start();


    //查询
    mypushbutton* lku = new mypushbutton(":/lookup.png");
    lku->setFixedSize(50,50);
    lku->setParent(this);
    lku->move(7, 80);
    QLabel * illust_lut = new QLabel;
    illust_lut->setParent(this);
    illust_lut->setFont(font);
    illust_lut->setText("详细查询");
    illust_lut->setGeometry(5,130,80,20);
    lookup * lookupscene = new lookup(this);
    connect(lku,&mypushbutton::clicked,[=]()
    {
        output_log("进行了详细查询");
        on_pause_clicked();
        this->paraGroup->pause();
        lku->zoom_up();
        lku->zoom_down();
        QTimer::singleShot(100,this,[=]()
        {
            lookupscene->map = this->map;
            lookupscene->table = this->table;
            lookupscene->vex = this->vex;
            lookupscene->risklist = this->risklist;
            lookupscene->namelist = this->namelist;
            lookupscene->time = day * 24 + hour;
            lookupscene->setGeometry(this->geometry());
            lookupscene->setWindowModality(Qt::WindowModal);
            lookupscene->show();
            emit lookupsig();
        });
    });
    connect(lookupscene,&lookup::back,this,[=]()
    {
       lookupscene->hide();
       this->log->insertPlainText("\n\n---查询了信息---\n\n");
       this->paraGroup->resume();
       on_pause_clicked();
    });



    //新城市
    mypushbutton* newct = new mypushbutton(":/city.png");
    newct->setFixedSize(50,50);
    newct->setParent(this);
    newct->move(7, 160);
    QLabel * illust_city = new QLabel;
    illust_city->setParent(this);
    illust_city->setFont(font);
    illust_city->setText("添加城市");
    illust_city->setGeometry(5,210,80,20);
    connect(newct,&mypushbutton::clicked,[=]()
    {
        isChanging = true;
        newct->zoom_up();
        newct->zoom_down();
        if(this->vex == 31)
        {
            isChanging = false;
            on_pause_clicked();
            paraGroup->pause();
            QMessageBox::critical(this, tr("Error"),tr("已达到最大城市数，无法继续添加"),QMessageBox::Ok);
            output_log("已达到最大城市数，无法继续添加");
            on_pause_clicked();
            paraGroup->resume();
        }
        else
        {
            on_pause_clicked();
            paraGroup->pause();
            QString str = "输入" + iter[this->vex] + "风险指数: ";
            bool ok;
            double b = QInputDialog::getDouble(this,"风险",str,0.2,0.2,0.9,1,&ok,Qt::WindowMaximizeButtonHint);
            if(ok == false)
            {
                 QMessageBox::warning(this, tr("Error"),tr("已取消此次操作"),QMessageBox::Ok);
                 output_log("已取消此次增加城市");
            }
            else
            {
                this->vex++;
                double* temp = this->risklist;
                this->risklist = new double[this->vex];

                for(int i = 0;i < this->vex-1;i++)
                {
                    this->risklist[i] = temp[i];
                }
                this->risklist[this->vex - 1] = b;
                path** tempmap = this->map;
                this->map = new path * [this->vex];
                elem** temptable = this->table;
                this->table = new elem * [this->vex];
                for (int i = 0; i < this->vex; i++)
                {
                    this->table[i] = new elem[this->vex];
                    this->map[i] = new path[this->vex];
                    if(i != this->vex - 1)
                    {
                        for (int k = 0; k < this->vex; k++)
                        {
                            if(k != this->vex - 1)
                            {
                                this->map[i][k].BusWeight = tempmap[i][k].BusWeight;
                                this->map[i][k].TrainWeight = tempmap[i][k].TrainWeight;
                                this->map[i][k].PlaneWeight = tempmap[i][k].PlaneWeight;
                                this->table[i][k].BusNext = temptable[i][k].BusNext;
                                this->table[i][k].TrainNext = temptable[i][k].TrainNext;
                                this->table[i][k].PlaneNext = temptable[i][k].PlaneNext;
                            }
                            else
                            {
                                this->map[i][k].BusWeight = -1;
                                this->map[i][k].TrainWeight = -1;
                                this->map[i][k].PlaneWeight = -1;
                                this->table[i][k].BusNext = NULL;
                                this->table[i][k].PlaneNext = NULL;
                                this->table[i][k].TrainNext = NULL;
                            }
                        }
                    }
                    else
                    {
                        for (int k = 0; k < this->vex; k++)
                        {
                            this->map[i][k].BusWeight = -1;
                            this->map[i][k].TrainWeight = -1;
                            this->map[i][k].PlaneWeight = -1;
                            this->table[i][k].BusNext = NULL;
                            this->table[i][k].PlaneNext = NULL;
                            this->table[i][k].TrainNext = NULL;
                        }
                    }
                }
                this->log->insertPlainText("\n\n---添加了新城市---\n\n");
                output_log("添加了新城市");
                isChanging = false;
            }
            on_pause_clicked();
            paraGroup->resume();
        }
    });


    //新时刻
    mypushbutton* newtb = new mypushbutton(":/schedule.png");
    newtb->setFixedSize(50,50);
    newtb->setParent(this);
    newtb->move(7,240);
    QLabel * illust_tb = new QLabel;
    illust_tb->setParent(this);
    illust_tb->setFont(font);
    illust_tb->setText("添加班次");
    illust_tb->setGeometry(5,290,80,20);
    newtable * newtbscene = new newtable;
    connect(newtb,&mypushbutton::clicked,[=]()
    {
        newtb->zoom_up();
        newtb->zoom_down();
        on_pause_clicked();
        paraGroup->pause();
        newtbscene->map = this->map;
        newtbscene->table = this->table;
        newtbscene->vex = this->vex;
        newtbscene->show();
    });
    connect(newtbscene,&newtable::back,this,[=]()
    {
       this->map = newtbscene->map;
       this->table = newtbscene->table;
       newtbscene->hide();
       this->log->insertPlainText("\n\n---更新了时刻表---\n\n");
       output_log("更新了时刻表");
       on_pause_clicked();
       paraGroup->resume();
    });


    //新旅客
    mypushbutton* addperson = new mypushbutton(":/addperson.png");
    addperson->setFixedSize(50,50);
    addperson->setParent(this);
    addperson->move(7,320);
    QLabel * illust_per = new QLabel;
    illust_per->setParent(this);
    illust_per->setFont(font);
    illust_per->setText("添加旅客");
    illust_per->setGeometry(5,370,80,20);
    connect(addperson,&mypushbutton::clicked,[=]()
    {
        addperson->zoom_up();
        addperson->zoom_down();
        on_pause_clicked();
        paraGroup->pause();
        int start;
        int mid;
        int end;
        int kind;
        int limit;
        int depart;
        int flag = 0;
        bool ok;
        int pernum = QInputDialog::getInt(this,"新增旅客数","请输入新增旅客数",1,1,10,1,&ok,Qt::WindowMaximizeButtonHint);
        if(ok == false)
        {
            QMessageBox::warning(this, tr("Error"),tr("已取消此次操作"),QMessageBox::Ok);
            output_log("已取消此次增加旅客");
        }
        else
        {
            for(int i =0;i<pernum;i++)
            {
                input_person dlg;
                dlg.setFixedSize(640,480);
                dlg.setvex(this->vex);
                QString str = QString("请输入第%1个旅客的信息").arg(i + 1);
                dlg.setWindowTitle(str);
                if (dlg.exec() == QDialog::Accepted)
                {
                    start = dlg.getstart();
                    end = dlg.getend();
                    kind = dlg.getkind();
                    depart = dlg.getdepart();
                    limit = dlg.getlimit();
                    mid = dlg.getmid();

                    person per(this->namelist.size(),start,mid,end,kind,depart,limit,this->vex,this->risklist,this->map,this->table);

                    if(per.pathexist==0)
                    {
                        QMessageBox::critical(this, tr("Error"),tr("不存在这样的路径，添加失败"),QMessageBox::Ok);
                        output_log("不存在满足旅客要求的的路径规划");
                    }
                    else if(depart < (this->day * 24 + this->hour))
                    {
                        QMessageBox::critical(this, tr("Error"),tr("出发时间不合法，添加失败"),QMessageBox::Ok);
                        output_log("输入的出发时间不合法，添加旅客失败");
                    }
                    else
                    {
                        flag = 1;
                        this->namelist.push_back(per);
                        pathdetail* showtemp = new pathdetail(this,per);
                        showtemp->show();
                        if (showtemp->exec() == QDialog::Accepted)
                        {}
                        if(per.chosenplan.size()==0)
                        {
                            this->namelist.back().chosenplan = per.bestplan[showtemp->index];
                        }
                    }
                }
                int cnt = this->namelist.size() - 1;
                if(flag == 1)
                {
                    person& per = this->namelist.back();
                    per.pic = new QLabel(this);    //旅客维护的label，用来显示图片
                    per.seqGroup = new QSequentialAnimationGroup(this);              //每个旅客内部维护一个seq group
                    connect(per.seqGroup,&QPropertyAnimation::finished,[=]()
                    {
                        delete per.pic;
                     });

                    per.seqGroup->addPause(10000 * per.departTime);
                    for (auto it = per.chosenplan.begin(); it != per.chosenplan.end(); it++)
                    {
                        QString transp;
                        if ((*it).transport == 'b')                                 //旅客下一阶段乘坐的交通工具图片
                        {
                            transp = ":/bus.png";
                        }
                        if ((*it).transport == 't')
                        {
                            transp = ":/train.png";
                        }
                        if ((*it).transport == 'p')
                        {
                            transp = ":/plane.png";
                        }
                        per.pic->setPixmap(transp);
                        int duration = (*it).nextcost;                              //旅行用时
                        QPropertyAnimation *animation = new QPropertyAnimation(per.pic, "geometry");//两城市之间的一个动画
                        per.seqGroup->addPause(10000 * (*it).staytime);               //要是旅客到了一个城市，还需要等车，给seq加一段暂停时间
                        animation->setStartValue(QRect(build[(*it).curcity - 1][0]-25,build[(*it).curcity - 1][1]-25,50,50));//在这一阶段的旅行中，起始城市坐标
                        animation->setEndValue(QRect(build[(*it).nextcity - 1][0]-25,build[(*it).nextcity - 1][1]-25,50,50));//终点城市坐标
                        animation->setDuration(duration * 10000);
                        connect(animation,&QPropertyAnimation::stateChanged,[=]()
                        {
                            this->namelist[cnt].pic->setPixmap(transp);
                            this->namelist[cnt].pic->show();
                         });
                        per.seqGroup->addAnimation(animation);                       //把这一小段旅程动画加到这个旅客的整个旅程的动画中去
                    }
                    this->paraGroup->addAnimation(per.seqGroup);                 //所有的旅客一起组成一个并行的组，方便一起暂停
                }
            }
            this->log->insertPlainText("\n\n---添加了新旅客---\n\n");
            output_log("添加了新旅客");
        }
        on_pause_clicked();
        paraGroup->resume();
    });



    //改路
    mypushbutton* changeway = new mypushbutton(":/changepath.png");
    changeway->setFixedSize(50,50);
    changeway->setParent(this);
    changeway->move(7,400);
    QLabel * illust_change = new QLabel;
    illust_change->setParent(this);
    illust_change->setFont(font);
    illust_change->setText("修改路径");
    illust_change->setGeometry(5,450,80,20);
    connect(changeway,&mypushbutton::clicked,[=](){
        changeway->zoom_up();
        changeway->zoom_down();
        on_pause_clicked();
        paraGroup->pause();
        bool ok;
        if(this->namelist.size() == 0)
        {
            QMessageBox::warning(this, tr("Error"),tr("当前旅客数为0"),QMessageBox::Ok);
            output_log("修改旅客计划失败，因为旅客数为0");
        }
        else
        {
            int pernum = QInputDialog::getInt(this,"修改路径","请输入要修改路径的旅客编号",1,1,10,1,&ok,Qt::WindowMaximizeButtonHint);
            if(ok == false)
            {
                QMessageBox::warning(this, tr("Error"),tr("已取消此次操作"),QMessageBox::Ok);
                output_log("已取消此次修改旅客计划");
            }
            else if(this->namelist[pernum - 1].done == true||this->namelist[pernum - 1].loc == this->namelist[pernum - 1].chosenplan.back().nextcity)
            {
                QMessageBox::warning(this, tr("Error"),tr("该旅客已结束旅行，不能修改"),QMessageBox::Ok);
                output_log("旅客已结束旅行，不能修改计划");
            }
            else if(this->namelist[pernum - 1].still == false)
            {
                QMessageBox::warning(this, tr("Error"),tr("旅客正在交通工具上，不能修改"),QMessageBox::Ok);
                output_log("旅客正在交通工具上，不能修改");
            }
            else
            {
                int start = this->namelist[pernum-1].loc;
                change_path dlg(NULL,start);
                dlg.setFixedSize(640,480);
                dlg.setvex(this->vex);
                QString str = QString("请修改信息");
                dlg.setWindowTitle(str);
                if (dlg.exec() == QDialog::Accepted)
                {
                    int flag = 0;
                    int end = dlg.getend();
                    int kind = dlg.getkind();
                    int depart = dlg.getdepart();
                    int limit = dlg.getlimit();
                    int mid = dlg.getmid();
                    person per(this->namelist.size(),start,mid,end,kind,depart,limit,this->vex,this->risklist,this->map,this->table);
                    if(per.pathexist==0)
                    {
                        QMessageBox::critical(this, tr("Error"),tr("不存在这样的路径，添加失败"),QMessageBox::Ok);
                        output_log("不存在满足旅客要求的路径规划");
                    }
                    else if(depart < (this->day * 24 + this->hour))
                    {
                        QMessageBox::critical(this, tr("Error"),tr("出发时间不合法，添加失败"),QMessageBox::Ok);
                        output_log("输入的出发时间不合法，添加旅客失败");
                    }
                    else                    //修改旅客的动画
                    {
                        flag = 1;
                        this->namelist[pernum - 1].bestplan.clear();
                        this->namelist[pernum - 1].chosenplan.clear();
                        paraGroup->removeAnimation(this->namelist[pernum - 1].seqGroup);
                        delete this->namelist[pernum - 1].seqGroup;
                        delete this->namelist[pernum - 1].pic;
                        pathdetail* showtemp = new pathdetail(this,per);
                        showtemp->show();
                        if (showtemp->exec() == QDialog::Accepted)
                        {}
                        if(per.chosenplan.size()==0)
                        {
                            this->namelist.back().chosenplan = per.bestplan[showtemp->index];
                        }
                        this->namelist[pernum - 1].chosenplan = per.chosenplan;
                        this->namelist[pernum - 1].departTime = per.departTime;
                    }
                    if(flag == 1)
                    {
                        person& per = this->namelist[pernum - 1];
                        per.pic = new QLabel(this);
                        per.seqGroup = new QSequentialAnimationGroup(this);
                        connect(per.seqGroup,&QPropertyAnimation::finished,[=]()
                        {
                            delete per.pic;
                         });

                        per.seqGroup->addPause(10000 * per.departTime);
                        for (auto it = per.chosenplan.begin(); it != per.chosenplan.end(); it++)
                        {
                            QString transp;
                            if ((*it).transport == 'b')
                            {
                                transp = ":/bus.png";
                            }
                            if ((*it).transport == 't')
                            {
                                transp = ":/train.png";
                            }
                            if ((*it).transport == 'p')
                            {
                                transp = ":/plane.png";
                            }
                            per.pic->setPixmap(transp);
                            int duration = (*it).nextcost;
                            QPropertyAnimation *animation = new QPropertyAnimation(per.pic, "geometry");
                            per.seqGroup->addPause(10000 * (*it).staytime);
                            animation->setStartValue(QRect(build[(*it).curcity - 1][0]-25,build[(*it).curcity - 1][1]-25,50,50));
                            animation->setEndValue(QRect(build[(*it).nextcity - 1][0]-25,build[(*it).nextcity - 1][1]-25,50,50));
                            animation->setDuration(duration * 10000);
                            connect(animation,&QPropertyAnimation::stateChanged,[=]()
                            {
                                this->namelist[pernum - 1].pic->setPixmap(transp);
                                this->namelist[pernum - 1].pic->show();
                             });
                            per.seqGroup->addAnimation(animation);
                        }
                        this->paraGroup->addAnimation(per.seqGroup);
                    }
                    this->log->insertPlainText("\n\n---修改了旅客计划---\n\n");
                    output_log("修改了旅客计划");
                }
            }
        }
        on_pause_clicked();
        paraGroup->resume();
    });
}

void run::paintEvent(QPaintEvent*)//重写绘图事件
{
    QPainter painter(this);
    QPixmap pix;
    QPen mypen,mypenb,mypent,mypenp;
    mypenb.setWidth(3);
    mypenb.setCapStyle(Qt::RoundCap);
    mypenb.setStyle(Qt::DotLine);
    mypenb.setColor(Qt::yellow);
    mypent.setWidth(3);
    mypent.setCapStyle(Qt::RoundCap);
    mypent.setStyle(Qt::DotLine);
    mypent.setColor(Qt::green);
    mypenp.setWidth(3);
    mypenp.setCapStyle(Qt::RoundCap);
    mypenp.setStyle(Qt::DotLine);
    mypenp.setColor(Qt::blue);
    pix.load(":/bkg.png");

    painter.drawPixmap(0,0,this->width(),this->height(),pix);//拉伸，填满窗口
    if(isChanging == false){
    for(int i =0;i<vex;i++)
    {
        for(int k = 0;k < vex;k++)
        {          
            if(map[i][k].PlaneWeight!=-1)//飞机线路用蓝色虚线表示
            {
                painter.setPen(mypenp);
                painter.drawLine(build[i][0], build[i][1], build[k][0], build[k][1]);
            }
            if(map[i][k].TrainWeight!=-1)//火车线路用绿色虚线表示
            {
                painter.setPen(mypent);
                painter.drawLine(build[i][0], build[i][1], build[k][0], build[k][1]);
            }
            if(map[i][k].BusWeight!=-1)//汽车线路用黄色虚线表示
            {
                painter.setPen(mypenb);
                painter.drawLine(build[i][0], build[i][1], build[k][0], build[k][1]);
            }
        }

        for(int j = 0;j<vex;j++)//给各城市按照风险值上色标记
        {
            if(risklist[i]==0.9)
            {
                mypen.setColor(Qt::red);
                mypen.setCapStyle(Qt::RoundCap);
                mypen.setWidth(15);
                painter.setPen(mypen);
                painter.drawPoint(QPoint(build[i][0],build[i][1]));
            }
            else if(risklist[i]==0.5)
            {
                mypen.setColor(Qt::yellow);
                mypen.setCapStyle(Qt::RoundCap);
                mypen.setWidth(15);
                painter.setPen(mypen);
                painter.drawPoint(QPoint(build[i][0],build[i][1]));
            }
            else if(risklist[i]==0.2)
            {
                mypen.setColor(Qt::green);
                mypen.setCapStyle(Qt::RoundCap);
                mypen.setWidth(15);
                painter.setPen(mypen);
                painter.drawPoint(QPoint(build[i][0],build[i][1]));
            }
        }
    }}
}

void run::timerEvent(QTimerEvent * ev)//重写超时事件
{

    //时钟控制
    if (ev->timerId() != m_timerId)
    {
        QWidget::timerEvent(ev);
        return;
    }
    QTime t(0,0);
    t = t.addMSecs(m_accumulator);
    if (m_timer.isValid())
    {
        t = t.addMSecs(m_timer.elapsed());
    }
    else
    {
        killTimer(m_timerId);
        m_timerId = -1;
    }

    //系统时间计算
    int temday = (t.minute()*60 + t.second())/240;
    int temhour = ((t.minute()*60 + t.second())/10)%24;


    //时间变动，状态更新
    if(day != temday || hour!=temhour)
    {
        day = temday;
        hour = temhour;
        QString date = "当前时间：第" + QString::number(day) + "天" + QString::number(hour) + "点\n";
        output_log(date);
        m_label->setText(date);
        this->log->insertPlainText(date);
        this->log->moveCursor(QTextCursor::End);
        int cnt = 1;
        int time = day * 24 + hour;
        for (auto i = namelist.begin(); i != namelist.end(); i++)
        {
            QString str,status,index;
            index = "\n第" + QString::number(cnt) + "号旅客状态：\n";
            this->log->insertPlainText(index);
            int flag;
            int start_to_move = (*i).departTime;
            for (auto it = (*i).chosenplan.begin(); it != (*i).chosenplan.end(); it++)//遍历旅客状态
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

                if (time > arrivetime)
                {
                    flag = 0;
                    continue;
                }
                else if (time < start_to_move)
                {
                    (*i).still = true;
                    (*i).loc = (*it).curcity;
                    risk = 0;
                    status = "还未出发\n当前累计风险值为" + QString::number(risk) + "\n";
                    this->log->insertPlainText(status);
                    break;
                }
                else if (time == arrivetime)
                {
                    (*i).still = true;
                    (*i).loc = (*it).nextcity;
                    risk = (*it).risk;
                    status = "刚好到达城市" + iter[(*it).nextcity - 1] + "\n当前累计风险值为" + QString::number(risk) + "\n";
                    this->log->insertPlainText(status);
                    break;
                }
                else if (time >= depart && time < arrivetime)
                {
                    (*i).still = false;
                    (*i).loc = (*it).curcity;
                    risk = (*it).risk - risklist[(*it).curcity - 1] * rate * (arrivetime - time);
                    status = "从城市" + iter[(*it).curcity - 1] + "乘坐" + str + "前往城市" + iter[(*it).nextcity - 1] + "\n当前累计风险值为" + QString::number(risk, 'f', 1) + "\n";//防止第0秒出发限时无穷小
                    this->log->insertPlainText(status);
                    break;
                }
                else if (time < depart && time >= stay)
                {
                    (*i).still = true;
                    (*i).loc = (*it).curcity;
                    risk = (*it).risk - risklist[(*it).curcity - 1] * ((arrivetime - time - (*it).nextcost) + (*it).nextcost * rate);
                    status = "在城市" + iter[(*it).curcity - 1] + "滞留中\n当前累计风险值为" + QString::number(risk, 'f', 1) + "\n";
                    this->log->insertPlainText(status);
                    break;
                }
            }
            if (flag == 0)
            {
                (*i).still = true;
                (*i).done = true;
                status = "已经到达目的地\n";
                this->log->insertPlainText(status);
            }
            output_log(index + status);
            cnt++;
        }
    }
}
