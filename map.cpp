#include "map.h"
#include "QPainter"
#include <unistd.h>
#include "iostream"
#include "QTimer"
#include <math.h>
#include <vector>
#include <stdlib.h>
#include "room.h"

//Globals to transfer over data when the widget is first created
extern int Area;
extern int RoomMin;
extern int RoomMax;
extern bool rectangular;

Map::Map(QWidget *parent) :
    QWidget(parent, Qt::Window)
{
    this->showMaximized();
    QTimer::singleShot(50, this, SLOT(stage1()));
}


Map::~Map()
{

}

//Used for the connection algorithm
struct connect {
  Room* r1;
  Room* r2;
} ;

//compare lines based on thier length
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

//Create each of the rooms of different shapes and sizes
void Map::stage1()
{
    int totalrooms=((RoomMax==RoomMin) ? RoomMax : rand()%(RoomMax-RoomMin)+RoomMin);
    std::cout<<"Rooms: "<<totalrooms<<std::endl;
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
        if(offsety>this->maximumHeight()-100)
        {
            offsety=0;
            offsetx=offsetx+100;
        }
        Room *troom = new Room(height,width,this);
        troom->setObjectName("Room"+i);
        troom->setGeometry(offsetx,offsety,height,width);
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
                if(x>(rooms.at(i)->pos().x()-roo->size().width())&&(x<(rooms.at(i)->pos().x()+rooms.at(i)->size().width())))
                {
                    //if y is between the room-roo height and room+height
                    if(y>(rooms.at(i)->pos().x()-roo->size().height())&&(y<rooms.at(i)->pos().y()+rooms.at(i)->size().height()))
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
                //QLine temp(roo->pos().x()+(roo->width()/2),roo->pos().y()+(roo->height()/2),too->pos().x()+(too->width()/2),too->pos().y()+(too->height()/2));
                //lines[p++]=temp;
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
    Room* included[rooms.size()];
    included[incpoint++]=lines[q].r1;
    included[incpoint++]=lines[q].r2;
    q++;
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
            if(!p1)
                included[incpoint++]=lines[q].r1;
            if(!p2)
                included[incpoint++]=lines[q].r2;
            q=1;
        }
    }
    std::cout<<"it atleast works!"<<std::endl;
}

