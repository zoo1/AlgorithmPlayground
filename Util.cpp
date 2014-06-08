#include "QString"
#include "Util.h"
#include "math.h"

//Creates the integer value out of the string if it cannot be translated to a string returns -1, returns -2 if the string is empty
int convert_value(QString translate)
{
    if(translate.length()==0)
        return -2;
    int i=0,total=0;
    int length=translate.length();
    for(;i<length;i++)
    {
        if(translate[i]==QChar('0'))
        {
        }
        else if(translate[i]==QChar('1'))
        {
            total=total+pow(10,length-i-1);
        }
        else if(translate[i]==QChar('2'))
        {
            total=total+pow(10,length-i-1)*2;
        }
        else if(translate[i]==QChar('3'))
        {
            total=total+pow(10,length-i-1)*3;
        }
        else if(translate[i]==QChar('4'))
        {
            total=total+pow(10,length-i-1)*4;
        }
        else if(translate[i]==QChar('5'))
        {
            total=total+pow(10,length-i-1)*5;
        }
        else if(translate[i]==QChar('6'))
        {
            total=total+pow(10,length-i-1)*6;
        }
        else if(translate[i]==QChar('7'))
        {
            total=total+pow(10,length-i-1)*7;
        }
        else if(translate[i]==QChar('8'))
        {
            total=total+pow(10,length-i-1)*8;
        }
        else if(translate[i]==QChar('9'))
        {
            total=total+pow(10,length-i-1)*9;
        }
        else
        {
            return -1;
        }
    }
    return total;
}

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
