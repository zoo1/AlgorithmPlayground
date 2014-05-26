#ifndef RECT_H
#define RECT_H
#include "qpolygon.h"

class Rect
{
public:
    Rect();
    Rect(int,int,int,int);
    QPolygon retrievepoly();
    void translate(int,int);
    int ceny();
    int cenx();
private:
    QPolygon poly;
    int centx,centy;
};

#endif // RECT_H
