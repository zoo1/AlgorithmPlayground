#include "map.h"
#include "qlabel.h"
#include "iostream"
#include "QTimer"
#include <math.h>
#include <vector>
#include <stdlib.h>
#include "room.h"
#include "hallway.h"
#include "Util.h"

//Globals to transfer over data when the widget is first created
extern int RoomMin;
extern int RoomMax;
extern bool rectangular,tunnels,circles;

Map::Map(QWidget *parent) :
    QWidget(parent, Qt::Window)
{
    this->showMaximized();
    QLabel *desc = new QLabel(this);
    desc->setObjectName("desc");
    desc->setGeometry(0,0,maximumWidth(),10);
    desc->show();
    QTimer::singleShot(50, this, SLOT(stage1()));
}

void Map::Display(QString out)
{
    QLabel *desc=findChild<QLabel *>("desc");
    desc->setText(out);
}


Map::~Map()
{

}

//Used to create hallways and add doors
void Map::createhallways(Room * r1, Room * r2)
{
    //Just a horizontal hallway is in need
    if(!((r1->y()<r2->y()+6&&r1->y()+r1->height()<r2->y()+6)||(r1->y()>(r2->y()-6+r2->height())&&(r1->y()+r1->height())>(r2->y()-6+r2->height()))))
    {
        int ypos;
        if(r1->y()>r2->y())
            ypos=r1->y();
        else
            ypos=r2->y();
        if(r1->x()<r2->x())
        {
            Hallway *hall = new Hallway(6,r2->x()-(r1->x()+r1->width()),false,this);
            hall->setGeometry(r1->x()+r1->width(),ypos,r2->x()-(r1->x()+r1->width()),6);
            hall->show();
            r1->addDoor(QLine(r1->width()-1,ypos+1-r1->y(),r1->width()-1,ypos+4-r1->y()));
            r1->update();
            r2->addDoor(QLine(0,ypos+1-r2->y(),0,ypos+4-r2->y()));
            r2->update();
        }
        else
        {
            Hallway *hall = new Hallway(6,r1->x()-(r2->x()+r2->width()),false,this);
            hall->setGeometry(r2->x()+r2->width(),ypos,r1->x()-(r2->x()+r2->width()),6);
            hall->show();
            r1->addDoor(QLine(0,ypos+1-r1->y(),0,ypos+4-r1->y()));
            r1->update();
            r2->addDoor(QLine(r2->width()-1,ypos+1-r2->y(),r2->width()-1,ypos+4-r2->y()));
            r2->update();
        }
    }
    //Just a vertical hallway is in need
    else if(!((r1->x()<r2->x()+6&&r1->x()+r1->width()<r2->x()+6)||(r1->x()>(r2->x()-6+r2->width())&&(r1->x()+r1->width()>(r2->x()-6+r2->width())))))
    {
        int xpos;
        if(r1->x()>r2->x())
            xpos=r1->x();
        else
            xpos=r2->x();

        if(r1->y()>r2->y())
        {
            Hallway *hall = new Hallway(r1->y()-(r2->y()+r2->height()),6,true,this);
            hall->setGeometry(xpos,r2->y()+r2->height(),6,r1->y()-(r2->y()+r2->height()));
            hall->show();
            r1->addDoor(QLine(xpos+1-r1->x(),0,xpos+4-r1->x(),0));
            r1->update();
            r2->addDoor(QLine(xpos+1-r2->x(),r2->height()-1,xpos+4-r2->x(),r2->height()-1));
            r2->update();
        }
        else
        {
            Hallway *hall = new Hallway(r2->y()-(r1->y()+r1->height()),6,true,this);
            hall->setGeometry(xpos,r1->y()+r1->height(),6,r2->y()-(r1->y()+r1->height()));
            hall->show();
            r1->addDoor(QLine(xpos+1-r1->x(),r1->height()-1,xpos+4-r1->x(),r1->height()));
            r1->update();
            r2->addDoor(QLine(xpos+1-r2->x(),0,xpos+4-r2->x(),0));
            r2->update();
        }
    }
    else
    {
        //horizontal hallways based off of r1
        Hallway *h1;
        bool dir1;
        if((r2->x()+r2->width()/2)>(r1->x()+r1->width()/2))
        {
            int horlength=abs((r1->x()+(r1->size().width()))-(r2->x()+(r2->size().width()/2)+3));
            Hallway *hall = new Hallway(6,horlength,false,this);
            h1=hall;
            dir1=true;
            hall->setGeometry(r1->x()+r1->size().width(),r1->y()+(r1->size().height()/2)-3,horlength,6);
            hall->show();
            r1->addDoor(QLine(r1->size().width()-1,(r1->size().height()/2)-2,r1->size().width()-1,(r1->size().height()/2)+1));
            r1->update();
        }
        else
        {
            int horlength=abs((r1->x())-(r2->x()+(r2->size().width()/2)-3));
            Hallway *hall = new Hallway(6,horlength,false,this);
            h1=hall;
            dir1=false;
            hall->setGeometry(r1->x()-horlength,r1->y()+(r1->size().height()/2)-3,horlength,6);
            hall->show();
            r1->addDoor(QLine(0,(r1->size().height()/2)-2,0,(r1->size().height()/2)+1));
            r1->update();
        }
        //vertical hallways based off of r2 heading up off of r2
        if((r1->y()+r1->height()/2)<(r2->y()+r2->height()/2))
        {
            int horlength=r2->y()-(r1->y()+(r1->size().height()/2)-3);
            Hallway *hall = new Hallway(horlength,6,true,this);
            hall->setGeometry(r2->x()+(r2->size().width()/2)-3,r2->y()-horlength,6,horlength);
            hall->show();
            r2->addDoor(QLine((r2->size().width()/2)-2,0,(r2->size().width()/2)+1,0));
            r2->update();
            //adding a door to both always to where they connect for upward hallway
            if(dir1)
            {
                (h1)->addDoor(QLine((h1)->width()-1,6,(h1)->width()-5,6));
                hall->addDoor(QLine(0,2,0,3));

            }
            else
            {
                (h1)->addDoor(QLine(0,6,4,6));
                hall->addDoor(QLine(6,2,6,3));
            }
        }
        //vertical hallway heading down off of r2
        else
        {
            int horlength=(r1->y()+(r1->size().height()/2)+3)-(r2->y()+r2->size().height());
            Hallway *hall = new Hallway(horlength,6,true,this);
            hall->setGeometry(r2->x()+(r2->size().width()/2)-3,r2->y()+r2->size().height(),6,horlength);
            hall->show();
            r2->addDoor(QLine((r2->width()/2)-2,r2->height()-1,(r2->width()/2)+1,r2->height()-1));
            r2->update();
            //adding a door to both always to where they connect for downward hallway
            if(dir1)
            {
                (h1)->addDoor(QLine((h1)->width()-1,0,(h1)->width()-5,0));
                hall->addDoor(QLine(0,hall->height()-3,0,hall->height()-4));

            }
            else
            {
                (h1)->addDoor(QLine(0,0,4,0));
                hall->addDoor(QLine(6,hall->height()-3,6,hall->height()-4));
            }
        }
    }
}

//Create each of the rooms of different shapes and sizes
void Map::stage1()
{
    int totalrooms=((RoomMax==RoomMin) ? RoomMax : rand()%(RoomMax-RoomMin)+RoomMin);
    // Quick print out of the map setup
    std::cout<<"Rooms: "<<totalrooms<<std::endl;
    std::cout<<"Room types: Squares";
    if(rectangular)
        std::cout<<" Rectangles";
    if(tunnels)
        std::cout<<" Tunnel";
    if(circles)
        std::cout<<" Circles";
    std::cout<<std::endl;
    int offsety=0;
    int offsetx=50;

    for(int i=0;i<totalrooms;i++)
    {
        int width,height;
        if(rectangular)
        {
            width=(rand()%40+9)*2;
            height=(rand()%40+9)*2;
        }
        else
        {
            width=height=(rand()%40+9)*2;
        }
        if(offsety>(this->height()-100))
        {
            offsety=0;
            offsetx=offsetx+100;
        }
        Room *troom = new Room(height,width,this);
        QString name=QString("Room %1").arg(i);
        troom->setObjectName(name);
        troom->setGeometry(offsetx,offsety,width,height);
        troom->show();
        offsety=offsety+height+2;
    }
    QTimer::singleShot(1000, this, SLOT(stage2()));

}

//Generate the random location check that no rooms overlap and that there isnt a spacing issue
void Map::stage2()
{
    QList<Room*> rooms=this->findChildren<Room*>();
    foreach(Room* roo,rooms)
    {
        int x=rand()%(size().width()-100)+5;
        int y=rand()%(size().height()-100)+5;
        bool isoverlap=true;
        while(isoverlap)
        {
            isoverlap=false;
            int i=0;
            while(roo!=rooms.at(i))
            {
                //if x is between the room-roo width and room+width
                if(x>(rooms.at(i)->x()-roo->width())&&(x<(rooms.at(i)->x()+rooms.at(i)->width())))
                {
                    //if y is between the room-roo height and room+height
                    if(y>(rooms.at(i)->y()-roo->height())&&(y<rooms.at(i)->y()+rooms.at(i)->height()))
                    {
                        isoverlap=true;
                        x=rand()%(size().width()-100)+5;
                        y=rand()%(size().height()-100)+5;
                    }
                }
                i++;
            }
        }
        roo->move(x,y);
    }
    QTimer::singleShot(1000, this, SLOT(stage3()));
}


void Map::stage3()
{

    int p=0;
    QList<Room*> rooms=this->findChildren<Room*>();
    struct connect lines[(rooms.length()*rooms.length())-rooms.length()];
    foreach(Room* roo,rooms)
    {
        foreach(Room* too,rooms)
        {
            if(roo!=too)
            {
                struct connect temp;
                temp.r1=roo;
                temp.r2=too;
                lines[p++]=temp;
            }
        }
    }
    //sort the lines and draw the lines in between them
    qsort(lines,(rooms.length()*rooms.length())-rooms.length(),sizeof(struct connect),compareconnect);
    int incpoint=0;
    int q=0;
    //just add in the first line as a base point
    lines[q].r1->addconnects(lines[q].r2);
    lines[q].r2->addconnects(lines[q].r1);
    createhallways(lines[q].r1,lines[q].r2);
    Room* included[rooms.size()];
    included[incpoint++]=lines[q].r1;
    included[incpoint++]=lines[q].r2;
    q++;
    //add connections between the rest of the rooms
    while(incpoint!=rooms.size()||q==(rooms.size()*rooms.size())-rooms.size()-1)
    {
        bool p1=false,p2=false;
        for(int i=0;i<incpoint;i++)
        {
            if(included[i]==lines[q].r1)
                p1=true;
            if(included[i]==lines[q].r2)
                p2=true;
        }
        if((p1&&p2)||(!p1&&!p2))
            q++;
        else
        {
            lines[q].r1->addconnects(lines[q].r2);
            lines[q].r2->addconnects(lines[q].r1);
            createhallways(lines[q].r1,lines[q].r2);
            if(!p1)
                included[incpoint++]=lines[q].r1;
            if(!p2)
                included[incpoint++]=lines[q].r2;
            q=1;
        }
    }
    QTimer::singleShot(1000, this, SLOT(stage4()));

}

//Begin setting up the dificulty analytics and the beginning and end locations
void Map::stage4()
{

}

