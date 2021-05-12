#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QLabel>
#include "mypushbutton.h"
#include "input_data.h"
#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include <QFile>
#include <QDir>
#include "run.h"
#include <QInputDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //设置固定大小
    setFixedSize(640,480);
    //设置图标
    setWindowIcon(QIcon(":/icon.png"));
    //设置标题
    setWindowTitle("COVID-19疫情环境下低风险旅行模拟系统");
    //设置字体
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(8);

    //读取文件
    mypushbutton* ReadDataBTN = new mypushbutton(":/file.png");
    ReadDataBTN->setFixedSize(50,50);
    ReadDataBTN->setParent(this);
    ReadDataBTN->move(135, 390);
    QLabel * ReadDataInstruct = new QLabel(this);
    ReadDataInstruct->setFont(font);
    ReadDataInstruct->setText("读取真实数据");
    ReadDataInstruct->setGeometry(120,440,80,20);
    connect(ReadDataBTN,&mypushbutton::clicked,[=]()
    {
        ReadDataBTN->zoom_up();
        ReadDataBTN->zoom_down();
        if(this->done == false)
        {

            //清空历史日志文件
            QFile log(QDir::currentPath()+"/log.txt");
            if (!log.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate))
            {
                return;
            }
            log.close();

            //读取数据文件
            QFile file(QDir::currentPath()+"/RealData.txt");
            int maxvex = 0;
            if(!file.open(QFile::ReadOnly | QFile::Text))
            {
                QMessageBox::warning(this,tr("Error"),tr("未找到数据文件"),QMessageBox::Ok);
            }
            else
            {               
                while (!file.atEnd())
                {
                    QByteArray line = file.readLine();
                    std::string str(line);
                    int from = str[0] - 'A' + 1;
                    int to = str[1] - 'A' + 1;
                    if(from>maxvex||to>maxvex)
                        maxvex = from>to?from:to;
                    if(str.size() < 5)
                    {
                        QMessageBox::warning(this,tr("Error"),tr("数据文件有误，请检查格式"),QMessageBox::Ok);
                        return;
                    }
                }
                file.close();
            }
            QFile file2(QDir::currentPath()+"/RealData.txt");
            if(!file2.open(QFile::ReadOnly | QFile::Text))
            {
                return;
            }
            else
            {
                //将编号转换为城市名
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


                //开辟存储空间
                this->vex = maxvex;
                this->map = new path * [this->vex];
                this->table = new elem * [this->vex];
                this->risklist = new double[this->vex];
                for(int i = 0;i<this->vex;i++)
                {
                    bool ok;
                    QString str = "输入" + iter[i] + "风险指数: ";
                    double b = QInputDialog::getDouble(this,"风险",str,0.2,0.2,0.9,1,&ok,Qt::WindowMaximizeButtonHint);
                    {
                        this->risklist[i] = b;
                    }
                    if(ok == false)
                    {
                        QMessageBox::warning(this, tr("Error"),tr("已取消此次输入，请重新输入"),QMessageBox::Ok);
                        i--;
                    }
                }
                for (int i = 0; i < this->vex; i++)
                {
                    table[i] = new elem[this->vex];
                    map[i] = new path[this->vex];
                    for (int k = 0; k < this->vex; k++)
                    {
                        map[i][k].BusWeight = -1;
                        map[i][k].TrainWeight = -1;
                        map[i][k].PlaneWeight = -1;
                        table[i][k].BusNext = NULL;
                        table[i][k].PlaneNext = NULL;
                        table[i][k].TrainNext = NULL;
                    }
                }

                //逐行处理数据，将其转换为时刻表
                while (!file2.atEnd())
                {
                    QByteArray line = file2.readLine();
                    std::string str(line);
                    dealdata(str);
                }
                file2.close();
                this->done = true;
                this->NoFile = false;
                QMessageBox::about(this, tr("返回到主界面"),tr("数据读入完成"));
            }
        }
        else
        {
            QMessageBox::warning(this,tr("Error"),tr("已经输入过信息了"),QMessageBox::Ok);
        }
     });

    //创建信息
    mypushbutton* InputDataBTN = new mypushbutton(":/keyboard.png");
    InputDataBTN->setFixedSize(50,50);
    InputDataBTN->setParent(this);
    InputDataBTN->move(295, 390);
    QLabel * IuputDataInstruct = new QLabel(this);
    IuputDataInstruct->setFont(font);
    IuputDataInstruct->setText("手动输入数据");
    IuputDataInstruct->setGeometry(280,440,80,20);
    input_data* InputDataScene = new input_data(this);
    connect(InputDataBTN,&mypushbutton::clicked,[=]()
    {
        InputDataBTN->zoom_up();
        InputDataBTN->zoom_down();
        if(this->done == false)
        {
            this->done = true;
            QTimer::singleShot(400,this,[=]()
            {
                InputDataScene->setGeometry(this->geometry());
                this->hide();
                InputDataScene->show();
                this->NoFile = false;
            });
        }
        else
        {
            QMessageBox::warning(this,tr("Error"),tr("已经输入过信息了"),QMessageBox::Ok);
        }
     });
    connect(InputDataScene,&input_data::back_to_main,this,[=]()
    {
       InputDataScene->hide();
       this->show();
    });


    //开始运行
    mypushbutton* RunBTN = new mypushbutton(":/start.png");
    RunBTN->setFixedSize(50,50);
    RunBTN->setParent(this);
    RunBTN->move(455, 390);
    QLabel * RunInstruct = new QLabel(this);
    RunInstruct->setFont(font);
    RunInstruct->setText("开始旅行模拟");
    RunInstruct->setGeometry(440,440,80,20);
    connect(RunBTN, &mypushbutton::clicked,[=]()
    {
        RunBTN->zoom_up();
        RunBTN->zoom_down();
        if(this->NoFile == true)
        {
            QMessageBox::warning(this, tr("Error"),tr("请先输入数据"),QMessageBox::Ok);
        }
        else
        {
            run* RunScene = new run(NULL,this->namelist,this->risklist,this->table,this->map,this->vex);//进入运行界面
            QTimer::singleShot(400,this,[=]()
            {
                this->hide();
                RunScene->show();
            });
        }
     });
}

//处理文件，转换成时刻表
void MainWindow::dealdata(std::string str)
{
    if(str.back()=='\n')
        str.pop_back();
    int len = str.size();
    int x = str[0] - 'A';//计算起始城市
    int y = str[1] - 'A';//结束城市
    int weight = str[3]-'0';
    if(str[2] == 'b')   //交通工具是汽车
    {
        map[x][y].BusWeight = weight;
        table[x][y].BusNext = new node;
        table[x][y].BusNext->start = -1;
        table[x][y].BusNext->next = NULL;
        for(int i = 4;i<len;i++)                //顺序插入时刻表
        {
            int cur_start = str[i] - 'a';
            node* ptr = table[x][y].BusNext;
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
    if(str[2] == 't')       //交通工具是火车
    {
        map[x][y].TrainWeight = weight;
        table[x][y].TrainNext = new node;
        table[x][y].TrainNext->start = -1;
        table[x][y].TrainNext->next = NULL;
        for(int i = 4;i<len;i++)
        {
            int cur_start = str[i] - 'a';
            node* ptr = table[x][y].TrainNext;
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
    if(str[2] == 'p')   //交通工具是飞机
    {
        map[x][y].PlaneWeight = weight;
        table[x][y].PlaneNext = new node;
        table[x][y].PlaneNext->start = -1;
        table[x][y].PlaneNext->next = NULL;
        for(int i = 4;i<len;i++)
        {
            int cur_start = str[i] - 'a';
            node* ptr = table[x][y].PlaneNext;
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
}
void MainWindow::paintEvent(QPaintEvent*)
{
    //背景图
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/bkg1.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);//拉伸，填满窗口
}

MainWindow::~MainWindow()
{
    delete ui;
}

