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
    painter.drawEllipse(QPoint(diameter/2,diameter/2),diameter/2,diameter/2);
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


Douhall::Douhall(int siz, int dir,int doorz, QWidget *parent)
    : Room (parent)
{
    issexit=isspawn=false;
    difficulty=0;
    direction=dir;
    size=siz;
    connects=doorz;
    s1=s2=false;

    if(connects==1)
    {
        if(direction)
        {
            addDoor(QLine(size/3,(size/2)-2,size/3,(size/2)+2));
            addDoor(QLine((2*size)/3,(size/2)-2,(2*size)/3,(size/2)+2));
        }
        else
        {
            addDoor(QLine((size/2)-2,size/3,(size/2)+2,size/3));
            addDoor(QLine((size/2)-2,(2*size)/3,(size/2)+2,(2*size)/3));
        }
    }
    if(connects==2)
    {
        if(direction)
        {
            addDoor(QLine(size/3,(size/4)-2,size/3,(size/4)+2));
            addDoor(QLine((2*size)/3,(size/4)-2,(2*size)/3,(size/4)+2));
            addDoor(QLine(size/3,3*(size/4)-2,size/3,3*(size/4)+2));
            addDoor(QLine((2*size)/3,3*(size/4)-2,(2*size)/3,3*(size/4)+2));
        }
        else
        {
            addDoor(QLine((size/4)-2,size/3,(size/4)+2,size/3));
            addDoor(QLine((size/4)-2,(2*size)/3,(size/4)+2,(2*size)/3));
            addDoor(QLine(3*(size/4)-2,size/3,3*(size/4)+2,size/3));
            addDoor(QLine(3*(size/4)-2,(2*size)/3,3*(size/4)+2,(2*size)/3));
        }
    }
}

void Douhall::setspawn()
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

void Douhall::setexit()
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

Douhall::~Douhall()
{

}

void Douhall::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPolygon box1;
    QPolygon box2;
    if(direction)
    {
        box1<<QPoint(0,0);
        box1<<QPoint(0,size);
        box1<<QPoint(size/3,size);
        box1<<QPoint(size/3,0);
        box2<<QPoint(size,0);
        box2<<QPoint(size,size);
        box2<<QPoint((2*size)/3,size);
        box2<<QPoint((2*size)/3,0);
    }
    else
    {
        box1<<QPoint(0,0);
        box1<<QPoint(size,0);
        box1<<QPoint(size,size/3);
        box1<<QPoint(0,size/3);
        box2<<QPoint(size,size);
        box2<<QPoint(0,size);
        box2<<QPoint(0,(2*size)/3);
        box2<<QPoint(size,(2*size)/3);
    }
    painter.drawPolygon(box1);
    painter.drawPolygon(box2);
    if(connects==1)
    {
        QPolygon temphall;
        if(direction)
        {
            temphall<<QPoint(size/3,(size/2)-3);
            temphall<<QPoint(size/3,(size/2)+3);
            temphall<<QPoint((2*size)/3,(size/2)+3);
            temphall<<QPoint((2*size)/3,(size/2)-3);
        }
        else
        {
            temphall<<QPoint((size/2)-3,size/3);
            temphall<<QPoint((size/2)+3,size/3);
            temphall<<QPoint((size/2)+3,(2*size)/3);
            temphall<<QPoint((size/2)-3,(2*size)/3);
        }
        painter.drawPolygon(temphall);
    }
    if(connects==2)
    {
        QPolygon temph1, temph2;
        if(direction)
        {
            temph1<<QPoint(size/3,(size/4)-3);
            temph1<<QPoint(size/3,(size/4)+3);
            temph1<<QPoint((2*size)/3,(size/4)+3);
            temph1<<QPoint((2*size)/3,(size/4)-3);
            temph2<<QPoint(size/3,3*(size/4)-3);
            temph2<<QPoint(size/3,3*(size/4)+3);
            temph2<<QPoint((2*size)/3,3*(size/4)+3);
            temph2<<QPoint((2*size)/3,3*(size/4)-3);
        }
        else
        {
            temph1<<QPoint((size/4)-3,size/3);
            temph1<<QPoint((size/4)+3,size/3);
            temph1<<QPoint((size/4)+3,(2*size)/3);
            temph1<<QPoint((size/4)-3,(2*size)/3);
            temph2<<QPoint(3*(size/4)-3,size/3);
            temph2<<QPoint(3*(size/4)+3,size/3);
            temph2<<QPoint(3*(size/4)+3,(2*size)/3);
            temph2<<QPoint(3*(size/4)-3,(2*size)/3);
        }
        painter.drawPolygon(temph1);
        painter.drawPolygon(temph2);
    }
    if(s1)
    {
        QPolygon temphall;
        if(direction)
        {
            temphall<<QPoint(size/3,0);
            temphall<<QPoint(size/3,6);
            temphall<<QPoint((2*size)/3,6);
            temphall<<QPoint((2*size)/3,0);
        }
        else
        {
            temphall<<QPoint(0,size/3);
            temphall<<QPoint(6,size/3);
            temphall<<QPoint(6,(2*size)/3);
            temphall<<QPoint(0,(2*size)/3);
        }
        painter.drawPolygon(temphall);
    }
    if(s2)
    {
        QPolygon temphall;
        if(direction)
        {
            temphall<<QPoint(size/3,size);
            temphall<<QPoint(size/3,size-6);
            temphall<<QPoint((2*size)/3,size-6);
            temphall<<QPoint((2*size)/3,size);
        }
        else
        {
            temphall<<QPoint(size,size/3);
            temphall<<QPoint(size-6,size/3);
            temphall<<QPoint(size-6,(2*size)/3);
            temphall<<QPoint(size,(2*size)/3);
        }
        painter.drawPolygon(temphall);
    }
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
