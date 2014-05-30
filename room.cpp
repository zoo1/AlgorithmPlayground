#include "room.h"
#include "QPainter"

Room::Room(QWidget *parent) :
    QWidget(parent)
{

}

Room::Room(int height, int width, QWidget *parent) :
    QWidget(parent)
{
    issexit=isspawn=false;
    poly << QPoint(0,0);
    poly << QPoint(0,height-1);
    poly << QPoint(width-1,height-1);
    poly << QPoint(width-1,0);
}

Room::~Room()
{

}

void Room::addDoor(QLine doo)
{
    doors.push_back(doo);
}

//if true sets a spawn in the room if false sets exit
void Room::setspot(bool spawnexit)
{
    if(spawnexit)
    {
        isspawn=true;
    }
    else
    {
        issexit=true;
    }

}

void Room::addconnects(Room *connect)
{
    connects.push_back(connect);
}

void Room::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPolygon(poly);
    QPen pen(Qt::red);
    pen.setWidth(3);
    painter.setPen(pen);
    for(int i=0;i<doors.size();i++)
    {
        painter.drawLine(doors[i]);
    }
}
