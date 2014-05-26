#include "map.h"
#include "ui_map.h"
#include "QPainter"
#include "rect.h"
#include <unistd.h>
#include "iostream"
#include "QTimer"
#include <math.h>
#include <vector>
#include <stdlib.h>

//Globals to transfer over data when the widget is first created
extern int Area;
extern int RoomMin;
extern int RoomMax;
extern bool rectangular;

//Globals to hold data throughout the generation process
static int currentstage=1;
static int totalrooms;
std::vector<Rect> rooms;

Map::Map(QWidget *parent) :
    QWidget(parent, Qt::Window)
{

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

void Map::paintEvent(QPaintEvent *e)
{
    totalrooms=((RoomMax==RoomMin) ? RoomMax : rand()%(RoomMax-RoomMin)+RoomMin);
    QPainter painter(this);
    QPen linepen;

    switch(currentstage)
    {
        //Rooms sizes are randomly generated and placed in order
        case 1:
        {
            std::cout<<"Rooms: "<<totalrooms<<std::endl;
            rooms.resize(0);
            int offset=0;
            int offsetx=100;
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
                offset=offset+(height)+2;
                if(offset>size().height()-100)
                {
                    offset=(height)+2;
                    offsetx=offsetx+100;
                }
                rooms.push_back(Rect(height,width,offsetx,offset-(height/2)));
            }

            painter.setPen(linepen);
            for(int i=0;i<totalrooms;i++)
            {
                painter.drawPolygon(rooms[i].retrievepoly());
            }
            currentstage=2;
            QTimer::singleShot(2000, this, SLOT(update()));
        }
        break;

        case 2:
        {
            //move the rooms to the specific map areas
            for(int i=0;i<rooms.size();i++)
            {
                int originx=-rooms[i].cenx(), originy=-rooms[i].ceny();

                rooms[i].translate(originx,originy);
                rooms[i].translate(rand()%(this->size().width()-100)+100,rand()%(this->size().height()-100)+100);
            }
            //redraw
            painter.setPen(linepen);
            for(int i=0;i<rooms.size();i++)
            {
                painter.drawPolygon(rooms[i].retrievepoly());
            }
            currentstage=3;
            QTimer::singleShot(2000, this, SLOT(update()));
        }
        break;

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
                    q++;
                }
            }
            QBrush tempbrush;
            tempbrush.setColor(Qt::red);
            tempbrush.setStyle(Qt::SolidPattern);

            for(int i=0;i<totalrooms;i++)
            {
                painter.drawPolygon(rooms[i].retrievepoly());
                QPainterPath path;
                path.addPolygon(rooms[i].retrievepoly());
                painter.fillPath(path,tempbrush);
            }
            //Cleanup
            currentstage=1;
            rooms.clear();

        }
        default:
            break;
    }
}




