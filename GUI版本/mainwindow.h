#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Struct.h"
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent*);
    QVector<person> namelist;
    double* risklist;
    path** map;
    elem** table;
    int vex;
signals:
private:
    void dealdata(std::string str);
    Ui::MainWindow *ui;
    bool done = false;//是否已经创建过信息
    bool NoFile = true;//防止没输入信息前运行
};
#endif // MAINWINDOW_H
