#include "generation.h"
#include "ui_generation.h"
#include <iostream>
#include "map.h"
#include "string"
#include "sstream"
#include "math.h"

//Defines
#define MIN 25
#define MAX 30
//Globals
bool errMin=true;
bool errMax=true;
bool squares=true;
bool rectangular=false;
bool tunnels=false;
bool circles=false;
int RoomMin=MIN;
int RoomMax=MAX;

Generation::Generation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Generation)
{
    ui->setupUi(this);
    //setting the default values in the text boxes
    ui->Standard->setChecked(true);
    std::string min = static_cast<std::ostringstream*>( &(std::ostringstream() << MIN) )->str();
    std::string max = static_cast<std::ostringstream*>( &(std::ostringstream() << MAX) )->str();
    ui->Roommin->setPlaceholderText(QString::fromStdString(min));
    ui->Roommax->setPlaceholderText(QString::fromStdString(max));
    ui->Tiny->setDisabled(true);
    ui->Maze->setDisabled(true);
    ui->checkBox_4->setChecked(true);
}

Generation::~Generation()
{
    delete ui;
}

void Generation::on_pushButton_clicked()
{
    if(errMin&&errMax)
    {
    mymap =new Map(this);
    mymap->showMaximized();
    }
    else
        std::cout<<"Error"<<std::endl;
}

void Generation::on_Roommin_textChanged()
{
    checkminmax();
}

void Generation::on_Roommax_textChanged()
{
    checkminmax();
}

void Generation::checkminmax()
{
    //grab both of the values in the plain text
    QString Maxstr = ui->Roommax->toPlainText();
    int Maxint = convert_value(Maxstr);
    QString Minstr = ui->Roommin->toPlainText();
    int Minint = convert_value(Minstr);
    //Both are none numbers
    if(Minint==-1&&Maxint==-1)
    {
        errMax=errMin=false;
        ui->Roommin->setStyleSheet("QTextEdit { background-color: rgb(255, 0, 0) }");
        ui->Roommax->setStyleSheet("QTextEdit { background-color: rgb(255, 0, 0) }");
    }
    //Min is greater than the max
    else if(Minint>0&&Maxint>0&&Minint>Maxint)
    {
        errMax=errMin=false;
        ui->Roommin->setStyleSheet("QTextEdit { background-color: rgb(255, 0, 0) }");
        ui->Roommax->setStyleSheet("QTextEdit { background-color: rgb(255, 0, 0) }");
    }
    //no input into either field
    else if((Maxint==-2&&Minint==-2))
    {
        errMax=errMin=true;
        ui->Roommin->setStyleSheet("QTextEdit { background-color: rgb(255, 255, 255) }");
        ui->Roommax->setStyleSheet("QTextEdit { background-color: rgb(255, 255, 255) }");
        RoomMin=MIN;
        RoomMax=MAX;
    }
    //overiding a default value
    else if((Minint==-2&&MIN>Maxint)||(Maxint==-2&&MAX<Minint))
    {
        errMax=errMin=false;
        ui->Roommin->setStyleSheet("QTextEdit { background-color: rgb(255, 0, 0) }");
        ui->Roommax->setStyleSheet("QTextEdit { background-color: rgb(255, 0, 0) }");
    }
    //One of the two are not numbers
    else if(Minint==-1&&Maxint!=-1)
    {
        errMin=false;
        errMax=true;
        ui->Roommin->setStyleSheet("QTextEdit { background-color: rgb(255, 0, 0) }");
        ui->Roommax->setStyleSheet("QTextEdit { background-color: rgb(255, 255, 255) }");
        if(Maxint==-2)
            RoomMax=MAX;
        else
            RoomMax=Maxint;
    }
    else if(Minint!=-1&&Maxint==-1)
    {
        errMax=false;
        errMin=true;
        ui->Roommax->setStyleSheet("QTextEdit { background-color: rgb(255, 0, 0) }");
        ui->Roommin->setStyleSheet("QTextEdit { background-color: rgb(255, 255, 255) }");
        if(Minint==-2)
            RoomMin=MIN;
        else
            RoomMin=Minint;
    }
    else
    {
        errMin=errMax=true;
        ui->Roommin->setStyleSheet("QTextEdit { background-color: rgb(255, 255, 255) }");
        ui->Roommax->setStyleSheet("QTextEdit { background-color: rgb(255, 255, 255) }");
        if(Minint==-2)
            RoomMin=MIN;
        else
            RoomMin=Minint;
        if(Maxint==-2)
            RoomMax=MAX;
        else
            RoomMax=Maxint;

    }
}

/** Check boxes for selecting each of the different room types. In order rectangular, tunnels, and circles
 *  Each changes a global which is passed over when a new map is created
 **/

void Generation::on_checkBox_4_clicked(bool checked)
{
    if(!checked&&!rectangular&&!tunnels&&!circles)
    {
        ui->checkBox_4->setChecked(!checked);
    }
    else
    {
        squares=(!squares);
    }
}

void Generation::on_checkBox_clicked(bool checked)
{
    if(!checked&&!squares&&!tunnels&&!circles)
    {
        ui->checkBox->setChecked(!checked);
    }
    else
    {
        rectangular=(!rectangular);
    }
}

void Generation::on_checkBox_2_clicked(bool checked)
{
    if(!checked&&!squares&&!rectangular&&!circles)
    {
        ui->checkBox_2->setChecked(!checked);
    }
    else
    {
        tunnels=(!tunnels);
    }
}

void Generation::on_checkBox_3_clicked(bool checked)
{
    if(!checked&&!squares&&!rectangular&&!tunnels)
    {
        ui->checkBox_3->setChecked(!checked);
    }
    else
    {
        circles=(!circles);
    }
}

//Creates the integer value out of the string if it cannot be translated to a string returns -1, returns -2 if the string is empty
int Generation::convert_value(QString translate)
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
