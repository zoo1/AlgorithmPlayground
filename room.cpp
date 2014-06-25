#include "room.h"
#include "QPainter"
#include <iostream>
#include "map.h"
#include "QStyle"

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

//Function sets spawn and then checks for the appropriate place to set the spawn
void Room::setspawn()
{
    isspawn=true;
}

//Function sets exit and then chexks for the appropriate place to set the exit
void Room::setexit()
{
    issexit=true;
}

void Room::addconnects(Room *connect)
{
    connects.push_back(connect);
}

std::vector<Room *> Room::getconnects()
{
    return connects;
}

void Room::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPolygon(poly);
    QPen pen(this->palette().background().color());
    painter.setPen(pen);
    for(int i=0;i<doors.size();i++)
    {
        painter.drawLine(doors[i]);
    }
    if(isspawn)
    {

    }
    if(issexit)
    {

    }
}

//Report to the parent widget the current details of the room
void Room::mousePressEvent(QMouseEvent *)
{
    QString name=QString::fromStdString("Name : %1. ").arg(objectName());
    QString sizz=QString::fromStdString("Size(H/W) : %1/").arg(size().height());
    QString sizz1=QString::fromStdString("%1. ").arg(size().width());
    QString doorz=QString::fromStdString("Doors : %1. ").arg(doors.size());
    ((Map *)parent())->Display(name.append(sizz).append(sizz1).append(doorz));
}
