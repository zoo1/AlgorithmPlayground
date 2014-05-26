#include "rect.h"
Rect::Rect()
{

}

Rect::Rect(int height,int width,int centerx,int centery)
{
    centy=centery;
    centx=centerx;
    poly << QPoint(centerx-(width/2),centery-(height/2));
    poly << QPoint(centerx-(width/2),centery+(height/2));
    poly << QPoint(centerx+(width/2),centery+(height/2));
    poly << QPoint(centerx+(width/2),centery-(height/2));
}

QPolygon Rect::retrievepoly()
{
    return poly;
}

void Rect::translate(int x, int y)
{
    poly.translate(x,y);
    centy=centy+y;
    centx=centx+x;
}

int Rect::ceny()
{
    return centy;
}

int Rect::cenx()
{
    return centx;
}



