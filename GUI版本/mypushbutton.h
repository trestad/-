#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class mypushbutton : public QPushButton
{
    Q_OBJECT
public:
    //explicit mypushbutton(QWidget *parent = nullptr);
    //构造函数，参数1：正常显示的图片路径，参数2：按下后显示的图片路径
    mypushbutton(QString normalIMG, QString pressIMG = "");
    //成员属性，保存用户传入的默认显示路径，以及按下后显示的图像路径
    QString normalImgPath;
    QString pressingImgPath;
    void zoom_up();
    void zoom_down();
    //重写按钮的按下和释放
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
signals:

};

#endif // MYPUSHBUTTON_H
