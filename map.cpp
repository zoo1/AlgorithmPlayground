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

//compare lines based on thier length
int compareQLine(const void * a, const void * b)
{
    int alength=sqrt(pow((*(QLine *)a).dy(),2)+pow((*(QLine*)a).dx(),2));
    int blength=sqrt(pow((*(QLine *)b).dy(),2)+pow((*(QLine*)b).dx(),2));
    if ( alength <  blength) return -1;
    if ( alength == blength ) return 0;
    if ( alength >  blength ) return 1;
}

void makehallways( QLine line ,QPolygon* hallway1 ,QPolygon* hallway2)
{
    //horizontal hallway
    if(line.x1()<line.x2())
    {
        (*hallway1)<<QPoint(line.x2()-3,line.y1()+3);
        (*hallway1)<<QPoint(line.x1()-3,line.y1()+3);
        (*hallway1)<<QPoint(line.x1()-3,line.y1()-3);
        (*hallway1)<<QPoint(line.x2()-3,line.y1()-3);
    }
    else
    {
        (*hallway1)<<QPoint(line.x2()+3,line.y1()+3);
        (*hallway1)<<QPoint(line.x1()+3,line.y1()+3);
        (*hallway1)<<QPoint(line.x1()+3,line.y1()-3);
        (*hallway1)<<QPoint(line.x2()+3,line.y1()-3);
    }
    //vertical hallway
    (*hallway1).translate(0,-line.y1()+line.y2());
    if(line.y1()<line.y2())
    {

        (*hallway2)<<QPoint(line.x1()-3,line.y2()-3);
        (*hallway2)<<QPoint(line.x1()-3,line.y1()-3);
        (*hallway2)<<QPoint(line.x1()+3,line.y1()-3);
        (*hallway2)<<QPoint(line.x1()+3,line.y2()-3);
    }
    else
    {
        (*hallway2)<<QPoint(line.x1()-3,line.y2()+3);
        (*hallway2)<<QPoint(line.x1()-3,line.y1()+3);
        (*hallway2)<<QPoint(line.x1()+3,line.y1()+3);
        (*hallway2)<<QPoint(line.x1()+3,line.y2()+3);
    }

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
        if(offsety>this->size().height()-100)
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
        roo->move(rand()%(size().width()-100)+100,rand()%(size().height()-100)+100);
    }
    QTimer::singleShot(1000, this, SLOT(stage3()));
}

void Map::stage3()
{

    int p=0;
    QList<Room*> rooms=this->findChildren<Room*>();
    QLine lines[(rooms.length()*rooms.length())-rooms.length()];
    foreach(Room* roo,rooms)
    {
        foreach(Room* too,rooms)
        {
            if(roo!=too)
            {
                QLine temp(roo->pos().x()+(roo->width()/2),roo->pos().y()+(roo->height()/2),too->pos().x()+(too->width()/2),too->pos().y()+(too->height()/2));
                lines[p++]=temp;
            }
        }
    }
    //sort the lines and draw the lines in between them
    qsort(lines,(rooms.length()*rooms.length())-rooms.length(),sizeof(QLine),compareQLine);
    int incpoint=0;
    int q=0;
    //just add in the first line as a base point
    QPolygon firsthallway;
    QPolygon secondhallway;
    makehallways(lines[q],&firsthallway,&secondhallway);
    painter.drawPolygon(firsthallway);
    painter.drawPolygon(secondhallway);
    QPoint included[totalrooms];
    included[incpoint++]=lines[q].p1();
    included[incpoint++]=lines[q].p2();
    q++;
    while(incpoint!=totalrooms||q==(totalrooms*totalrooms)-totalrooms-1)
    {
        bool p1=false,p2=false;
        for(int i=0;i<incpoint;i++)
        {
            if(included[i]==lines[q].p1())
                p1=true;
            if(included[i]==lines[q].p2())
                p2=true;
        }
        if((p1&&p2)||(!p1&&!p2))
            q++;
        else
        {
            QPolygon po1,po2;
            makehallways(lines[q],&po1,&po2);
            painter.drawPolygon(po1);
            painter.drawPolygon(po2);
            if(!p1)
                included[incpoint++]=lines[q].p1();
            if(!p2)
            included[incpoint++]=lines[q].p2();
            q=1;
        }
    }
    QBrush tempbrush;
    tempbrush.setColor(Qt::red);
    tempbrush.setStyle(Qt::SolidPattern);

    for(int i=0;i<rooms.size();i++)
    {
        painter.drawPolygon(rooms[i].retrievepoly());
        QPainterPath path;
        path.addPolygon(rooms[i].retrievepoly());
        painter.fillPath(path,tempbrush);
    }
}

/*



        //connect all the rooms via shortest path graph, this is a final stage so it also resets globals
        case 3:
        {
            painter.setPen(linepen);
            QLine lines[(totalrooms*totalrooms)-totalrooms];
            int p=0;
            for(int i=0;i<totalrooms;i++)
            {
                for(int j=0;j<totalrooms;j++)
                {
                    if(i!=j)
                    {
                        QLine temp(rooms[i].cenx(),rooms[i].ceny(),rooms[j].cenx(),rooms[j].ceny());
                        lines[p++]=temp;
                    }
                }
            }
            //sort the lines and draw the lines in between them
            qsort(lines,(totalrooms*totalrooms)-totalrooms,sizeof(QLine),compareQLine);
            int incpoint=0;
            int q=0;
            //just add in the first line as a base point
            QPolygon firsthallway;
            QPolygon secondhallway;
            makehallways(lines[q],&firsthallway,&secondhallway);
            painter.drawPolygon(firsthallway);
            painter.drawPolygon(secondhallway);
            QPoint included[totalrooms];
            included[incpoint++]=lines[q].p1();
            included[incpoint++]=lines[q].p2();
            q++;
            while(incpoint!=totalrooms||q==(totalrooms*totalrooms)-totalrooms-1)
            {
                bool p1=false,p2=false;
                for(int i=0;i<incpoint;i++)
                {
                    if(included[i]==lines[q].p1())
                        p1=true;
                    if(included[i]==lines[q].p2())
                        p2=true;
                }
                if((p1&&p2)||(!p1&&!p2))
                    q++;
                else
                {
                    QPolygon po1,po2;
                    makehallways(lines[q],&po1,&po2);
                    painter.drawPolygon(po1);
                    painter.drawPolygon(po2);
                    if(!p1)
                        included[incpoint++]=lines[q].p1();
                    if(!p2)
                    included[incpoint++]=lines[q].p2();
                    q=1;
                }
            }
            QBrush tempbrush;
            tempbrush.setColor(Qt::red);
            tempbrush.setStyle(Qt::SolidPattern);

            for(int i=0;i<rooms.size();i++)
            {
                painter.drawPolygon(rooms[i].retrievepoly());
                QPainterPath path;
                path.addPolygon(rooms[i].retrievepoly());
                painter.fillPath(path,tempbrush);
            }
        }
        default:
            break;
    }
    std::cout<<"stage ended"<<std::endl;
}
*/




