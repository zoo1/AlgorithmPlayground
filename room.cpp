#include "room.h"
#include "QPainter"

Room::Room(QWidget *parent) :
    QWidget(parent)
{

}

Room::Room(int height, int width, QWidget *parent) :
    QWidget(parent)
{
    poly << QPoint(0,0);
    poly << QPoint(0,height-1);
    poly << QPoint(width-1,height-1);
    poly << QPoint(width-1,0);
}

Room::~Room()
{

}

void Room::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPolygon(poly);
}
void Room::translate(int x, int y)
{
    poly.translate(x,y);
    centy=centy+y;
    centx=centx+x;
}

int Room::ceny()
{
    return centy;
}

int Room::cenx()
{
    return centx;
}
