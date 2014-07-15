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
    difficulty=0;
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
    if(issexit)
        std::cout<<"Error: Attepting to set a exit room to a spawn room"<<std::endl;
    isspawn=true;
    int i=0;
    bool side[4]={false,false,false,false};
    for(;i<doors.size();i++)
    {
        QLine temp=doors.at(i);
        if(temp.x1()==0||temp.x2()==0)
            side[0]=true;
        else if(temp.y1()==0||temp.y1()==0)
            side[1]=true;
        else if(temp.x1()==width()-1||temp.x2()==width()-1)
            side[2]=true;
        else if(temp.y1()==height()-1||temp.y2()==height()-1)
            side[3]=true;
    }
    i=0;
    while(side[i++]);
    is=i-1;
}

//Function sets exit and then checks for the appropriate place to set the exit
void Room::setexit()
{
    if(isspawn)
        std::cout<<"Error: Attepting to set a spawn room to a exit room"<<std::endl;
    issexit=true;
    int i=0;
    bool side[4]={false,false,false,false};
    for(;i<doors.size();i++)
    {
        QLine temp=doors.at(i);
        if(temp.x1()==0||temp.x2()==0)
            side[0]=true;
        else if(temp.y1()==0||temp.y1()==0)
            side[1]=true;
        else if(temp.x1()==width()-1||temp.x2()==width()-1)
            side[2]=true;
        else if(temp.y1()==height()-1||temp.y2()==height()-1)
            side[3]=true;
    }
    i=0;
    while(side[i++]);
    is=i-1;
}

void Room::addconnects(Room *connect)
{
    connects.push_back(connect);
}

void Room::setdifficulty(int dif)
{
    difficulty=dif;
}

int Room::getdifficulty()
{
    return difficulty;
}

std::vector<Room *> Room::getconnects()
{
    return connects;
}

void Room::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPolygon(poly);
    if(isspawn||issexit)
    {
        QPolygon box;
        switch(is)
        {
        case 0:
            box<<QPoint(0,(height()/2)-3);
            box<<QPoint(0,(height()/2)+3);
            box<<QPoint(6,(height()/2)+3);
            box<<QPoint(6,(height()/2)-3);
            break;
        case 1:
            box<<QPoint((width()/2)-3,0);
            box<<QPoint((width()/2)+3,0);
            box<<QPoint((width()/2)+3,6);
            box<<QPoint((width()/2)-3,6);
            break;
        case 2:
            box<<QPoint(width()-1,(height()/2)-3);
            box<<QPoint(width()-1,(height()/2)+3);
            box<<QPoint(width()-7,(height()/2)+3);
            box<<QPoint(width()-7,(height()/2)-3);
            break;
        case 3:
            box<<QPoint((width()/2)-3,height()-1);
            box<<QPoint((width()/2)+3,height()-1);
            box<<QPoint((width()/2)+3,height()-7);
            box<<QPoint((width()/2)-3,height()-7);
            break;
        }
        QBrush brush;
        if(isspawn)
        {
            brush.setColor(Qt::green);
            brush.setStyle(Qt::SolidPattern);
        }
        else
        {
            brush.setColor(Qt::red);
            brush.setStyle(Qt::SolidPattern);
        }
        // Fill polygon
        QPainterPath path;
        path.addPolygon(box);
        // Draw polygon
        painter.fillPath(path, brush);
        painter.drawPolygon(box);
    }
    QPen pen(palette().background().color());
    painter.setPen(pen);
    for(int i=0;i<doors.size();i++)
    {
        painter.drawLine(doors[i]);
    }

}

//Report to the parent widget the current details of the room
void Room::mousePressEvent(QMouseEvent *)
{
    QString name=QString::fromStdString("Name : %1. ").arg(objectName());
    QString sizz=QString::fromStdString("Size(H/W) : %1/").arg(size().height());
    QString sizz1=QString::fromStdString("%1. ").arg(size().width());
    QString doorz=QString::fromStdString("Doors : %1. ").arg(doors.size());
    QString diff=QString::fromStdString("Difficulty : %1. ").arg(difficulty);
    ((Map *)parent())->Display(name.append(sizz).append(sizz1).append(doorz).append(diff));
}
