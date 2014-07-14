#include "QString"
#include "Util.h"
#include "math.h"



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
