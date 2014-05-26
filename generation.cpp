#include "generation.h"
#include "ui_generation.h"
#include "Util.h"
#include <iostream>
#include "map.h"

//Globals
bool errArea=true;
bool errMin=true;
bool errMax=true;
bool rectangular=false;
int Area=2500;
int RoomMin=5;
int RoomMax=10;

Generation::Generation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Generation)
{
    ui->setupUi(this);
    //setting the default values in the text boxes
    ui->Area->setPlaceholderText("2500");
    ui->Roommin->setPlaceholderText("5");
    ui->Roommax->setPlaceholderText("10");
}

Generation::~Generation()
{
    delete ui;
}

void Generation::on_pushButton_clicked()
{
    if(errMin&&errMax&&errArea)
    {
    mymap =new Map(this);
    mymap->showMaximized();
    }
    else
        std::cout<<"Error"<<std::endl;
}

void Generation::on_Area_textChanged()
{
    QString Areastr = ui->Area->toPlainText();
    int Areaint=convert_value(Areastr);
    //paint the textedit white if it has the correct text in it
    if(Areaint!=-1)
    {
        errArea=true;
        ui->Area->setStyleSheet("QTextEdit { background-color: rgb(255, 255, 255) }");
    }
    //paint it red if it doesn't
    else if(Areaint==-1)
    {
        errArea=false;
        ui->Area->setStyleSheet("QTextEdit { background-color: rgb(255, 0, 0) }");
    }
    //if it is empty resore default
    else if(Areaint!=-2)
    {
        Area=2500;
    }
}

void Generation::on_Roommin_textChanged()
{
    checkminmax();
}


void Generation::on_Roommax_textChanged()
{
    checkminmax();
}

void Generation::on_checkBox_clicked()
{
    rectangular=(!rectangular);
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
        RoomMin=5;
        RoomMax=10;
    }
    //overiding a default value
    else if((Minint==-2&&5>Maxint)||(Maxint==-2&&10<Minint))
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
            RoomMax=10;
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
            RoomMin=5;
        else
            RoomMin=Minint;
    }
    else
    {
        errMin=errMax=true;
        ui->Roommin->setStyleSheet("QTextEdit { background-color: rgb(255, 255, 255) }");
        ui->Roommax->setStyleSheet("QTextEdit { background-color: rgb(255, 255, 255) }");
        if(Minint==-2)
            RoomMin=5;
        else
            RoomMin=Minint;
        if(Maxint==-2)
            RoomMax=10;
        else
            RoomMax=Maxint;

    }
}
