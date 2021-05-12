#include "mypushbutton.h"
#include <QPropertyAnimation>
mypushbutton::mypushbutton(QString normalIMG, QString pressIMG)
{
    this->normalImgPath = normalIMG;
    this->pressingImgPath = pressIMG;

    QPixmap pix;
    bool ret = pix.load(normalIMG);
    if(!ret)//如果加载图片失败
    {
        return;
    }

    //设置图片固定大小，一定要设置，不然不显示图片
    this->setFixedSize(pix.width(),pix.height());

    //设置不规则样式,去掉周围，不写这一行是方按钮
    this->setStyleSheet("QPushButton{border:0px;}");

    //设置图标，不写没有图标
    this->setIcon(pix);

    //设置图标大小，不写图标大小奇怪
    this->setIconSize(QSize(pix.width(),pix.height()));

}

void mypushbutton::zoom_up()
{
    //创建动态对象
    QPropertyAnimation * animation = new QPropertyAnimation(this,"geometry");
    //设置动画时间间隔
    animation->setDuration(200);
    //起始位置
    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    //结束位置
    animation->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    //设置弹跳曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);
    //执行动画
    animation->start();
}
void mypushbutton::zoom_down()
{
    //创建动态对象
    QPropertyAnimation * animation = new QPropertyAnimation(this,"geometry");
    //设置动画时间间隔
    animation->setDuration(200);
    //起始位置
    animation->setStartValue(QRect(this->x(),this->y() + 10,this->width(),this->height()));
    //结束位置
    animation->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));
    //设置弹跳曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);
    //执行动画
    animation->start();
}
void mypushbutton::mousePressEvent(QMouseEvent *e)
{
    if(this->pressingImgPath!=NULL)//传入的按下图片不为空，切换图片
    {

        QPixmap pix;
        bool ret = pix.load(this->pressingImgPath);
        if(!ret)//如果加载图片失败
        {
            return;
        }

        //设置图片固定大小，一定要设置，不然不显示图片
        this->setFixedSize(pix.width(),pix.height());

        //设置不规则样式,去掉周围，不写这一行是方按钮
        this->setStyleSheet("QPushButton{border:0px;}");

        //设置图标，不写没有图标
        this->setIcon(pix);

        //设置图标大小，不写图标大小奇怪
        this->setIconSize(QSize(pix.width(),pix.height()));
    }

    //让父类执行其他内容
    return QPushButton::mousePressEvent(e);
}

void mypushbutton::mouseReleaseEvent(QMouseEvent *e)
{
    if(this->pressingImgPath!=NULL)//传入的按下图片不为空，切换初始图片
    {

        QPixmap pix;
        bool ret = pix.load(this->normalImgPath);
        if(!ret)//如果加载图片失败
        {
            return;
        }

        //设置图片固定大小，一定要设置，不然不显示图片
        this->setFixedSize(pix.width(),pix.height());

        //设置不规则样式,去掉周围，不写这一行是方按钮
        this->setStyleSheet("QPushButton{border:0px;}");

        //设置图标，不写没有图标
        this->setIcon(pix);

        //设置图标大小，不写图标大小奇怪
        this->setIconSize(QSize(pix.width(),pix.height()));
    }

    //让父类执行其他内容
    return QPushButton::mouseReleaseEvent(e);
}
