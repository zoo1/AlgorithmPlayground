//will return -1 if the value is invalid, -2 if it was empty
#include "QString"
#include "math.h"
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
