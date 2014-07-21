#include "roomchildren.h"
#include <iostream>
#include "QPainter"

Block::Block(int height, int width, QWidget *parent) :
    Room (parent)
{
    issexit=isspawn=false;
    poly << QPoint(0,0);
    poly << QPoint(0,height-1);
    poly << QPoint(width-1,height-1);
    poly << QPoint(width-1,0);
    difficulty=0;
}

void Block::setspawn()
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

void Block::setexit()
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

Block::Block(int x, QWidget *parent) :
    Room (parent)
{
    issexit=isspawn=false;
    poly << QPoint(0,0);
    poly << QPoint(0,x-1);
    poly << QPoint(x-1,x-1);
    poly << QPoint(x-1,0);
    difficulty=0;
}

Block::~Block()
{

}

void Block::paintEvent(QPaintEvent *e)
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



Circle::Circle(int x, QWidget *parent)
    : Room (parent)
{
    issexit=isspawn=false;
    difficulty=0;
    diameter=x;
}

void Circle::setspawn()
{
    if(issexit)
        std::cout<<"Error: Attepting to set a exit room to a spawn room"<<std::endl;
    isspawn=true;
}

void Circle::setexit()
{
    if(isspawn)
        std::cout<<"Error: Attepting to set a spawn room to a exit room"<<std::endl;
    issexit=true;
}

Circle::~Circle()
{

}

void Circle::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    //Connecting Hallways
    for(int i=0;i<doors.size();i++)
    {
        QPolygon *ame=new QPolygon();
        QLine door=doors[i];

        if(door.dx()==0)
        {
            if(door.p1().y()>door.p2().y())
            {
                door.setP1(QPoint(door.p1().x(),door.p1().y()+1));
                door.setP2(QPoint(door.p2().x(),door.p2().y()-1));
            }
            else
            {
                door.setP1(QPoint(door.p1().x(),door.p1().y()-1));
                door.setP2(QPoint(door.p2().x(),door.p2().y()+1));
            }
            ame->append(door.p1());
            ame->append(door.p2());
            ame->append(QPoint(diameter/2,door.p2().y()));
            ame->append(QPoint(diameter/2,door.p1().y()));
        }
        if(door.dy()==0)
        {
            if(door.p1().x()>door.p2().x())
            {
                door.setP1(QPoint(door.p1().x()+1,door.p1().y()));
                door.setP2(QPoint(door.p2().x()-1,door.p2().y()));
            }
            else
            {
                door.setP1(QPoint(door.p1().x()-1,door.p1().y()));
                door.setP2(QPoint(door.p2().x()+1,door.p2().y()));
            }
            ame->append(door.p1());
            ame->append(door.p2());
            ame->append(QPoint(door.p2().x(),diameter/2));
            ame->append(QPoint(door.p1().x(),diameter/2));
        }
        painter.drawPolygon(*ame);
    }
    //Round room
    painter.setBrush(palette().background().color());
    painter.drawEllipse(QPoint(diameter/2,diameter/2),diameter/2-1,diameter/2-1);
    //Hallway Clean up
    for(int i=0;i<doors.size();i++)
    {
        QPolygon *erase=new QPolygon();
        QLine door=doors[i];
        painter.setPen(palette().background().color());
        if(door.dy()==0)
        {
            erase->append(door.p1());
            erase->append(door.p2());
            erase->append(QPoint(door.p2().x(),diameter/2));
            erase->append(QPoint(door.p1().x(),diameter/2));
        }
        if(door.dx()==0)
        {
            erase->append(door.p1());
            erase->append(door.p2());
            erase->append(QPoint(diameter/2,door.p2().y()));
            erase->append(QPoint(diameter/2,door.p1().y()));
        }
        painter.drawPolygon(*erase);
    }
    painter.setPen(Qt::black);
    //Exit and Entrance
    if(isspawn||issexit)
    {
        if(issexit)
            painter.setBrush(Qt::red);
        else
            painter.setBrush(Qt::green);
        painter.drawEllipse(QPoint(diameter/2,diameter/2),3,3);
    }
}
