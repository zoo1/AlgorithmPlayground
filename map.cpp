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
#include "math.h"

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

/**
  Stage functions: Each function is a differnent step in the generation procedure.
  These generation steps are seperated by a 1 second wait time
    Stage 1: Create each of the rooms of different shapes and sizes.
    Stage 2: Generate the random location for each room and verify no overlap occurs.
    Stage 3: Create hallways connecting all of the rooms together. This makes sure that
        you can travel from any one room to another.
    Stage 4: Difficulty analytics,the beginning and end locations, and removing hallway overlap
  **/

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

void Map::stage4()
{
    //Pick spawn room and DFS set difficulty
    QList<Room*> rooms=this->findChildren<Room*>();
    int i=0,difficulty=1;
    while(rooms.at(i++)->getconnects().size()>1);
    rooms.at(i-1)->setspawn();
    rooms.at(i-1)->setdifficulty(difficulty);
    rooms.at(i-1)->update();
    std::vector<Room *> pass;
    pass.push_back(rooms.at(i-1));
    recursiveDif(pass,difficulty);
    //Pick exit room
    int j=0,maxdifficulty=0;
    for(i=0;i<rooms.size();i++)
    {
        if(rooms[i]->getdifficulty()>maxdifficulty)
        {
            j=i;
            maxdifficulty=rooms[i]->getdifficulty();
        }
    }
    rooms[j]->setexit();
    rooms[j]->update();
    //Remove hallways overlapping rooms
    QList<Hallway*> hallways=this->findChildren<Hallway*>();
    for(i=0;i<rooms.size();i++)
    {
        for(j=0;j<hallways.size();j++)
        {
            //Horizontal Overlap
            if(!(rooms[i]->x()>=(hallways[j]->x()+hallways[j]->width())||(rooms[i]->x()+rooms[i]->width())<=hallways[j]->x()))
            {
                //Vertical Overlap
                if(!(rooms[i]->y()>=(hallways[j]->y()+hallways[j]->height())||(rooms[i]->y()+rooms[i]->height())<=hallways[j]->y()))
                {
                    removeoverlap(rooms[i],hallways[j]);
                }
            }
        }
    }
}

/**
  Helper function: These are helper functions for the stage functions.
    createhallways: Used to create hallways and add doors.
    recursiveDif: A recursive function which iterates through the rooms and marks the difficulty of the room.
    removeoverlap: Removes the portions of a hallway which have overlapped into a room.
    compareconnect: Compares lines based on thier length.
  **/

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

void Map::recursiveDif(std::vector<Room *> nodes, int difficulty)
{
    std::vector<Room*> next;
    int i=0,j=0;
    difficulty++;
    for(;j<nodes.size();j++)
    {
        std::vector<Room*> conn=nodes.at(j)->getconnects();
        for(i=0;i<conn.size();i++)
        {
            if(conn.at(i)->getdifficulty()<1)
            {
                conn.at(i)->setdifficulty(difficulty);
                next.push_back(conn.at(i));
            }
        }
    }
    if(next.size()!=0)
    {
        recursiveDif(next,difficulty);
    }
}

void Map::removeoverlap(Room *, Hallway *)
{

}

int compareconnect(const void * a, const void * b)
{
    struct connect t1=*(struct connect*)a;
    struct connect t2=*(struct connect*)b;
    int dx1=(t1.r1->pos().x()+(t1.r1->width()/2))-(t1.r2->pos().x()+(t1.r2->width()/2));
    int dy1=(t1.r1->pos().y()+(t1.r1->height()/2))-(t1.r2->pos().y()+(t1.r2->height()/2));
    int dx2=(t2.r1->pos().x()+(t1.r1->width()/2))-(t2.r2->pos().x()+(t1.r2->width()/2));
    int dy2=(t2.r1->pos().y()+(t1.r1->height()/2))-(t2.r2->pos().y()+(t1.r2->height()/2));
    int alength=sqrt(pow(dx1,2)+pow(dy1,2));
    int blength=sqrt(pow(dx2,2)+pow(dy2,2));
    if ( alength <  blength) return -1;
    if ( alength == blength ) return 0;
    if ( alength >  blength ) return 1;
}
