#include "hallway.h"
#include "QPainter"

Hallway::Hallway(QWidget *parent) :
    QWidget(parent)
{

}
//horizontal if bool is true, vertical if false
Hallway::Hallway(int height, int width,bool dir, QWidget *parent) :
    QWidget(parent)
{
    if (dir)
    {
        wall1=QLine(QPoint(0,0),QPoint(0,height-1));
        wall2=QLine(QPoint(width-1,height-1),QPoint(width-1,0));
    }
    else
    {
        wall1=QLine(QPoint(0,0),QPoint(width-1,0));
        wall2=QLine(QPoint(width-1,height-1),QPoint(0,height-1));
    }
}

Hallway::~Hallway()
{

}

void Hallway::addDoor(QLine doo)
{
    doors.push_back(doo);
}

void Hallway::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawLine(wall1);
    painter.drawLine(wall2);
    QPen pen(Qt::red);
    pen.setWidth(3);
    painter.setPen(pen);
    for(int i=0;i<doors.size();i++)
    {
        painter.drawLine(doors[i]);
    }
}
