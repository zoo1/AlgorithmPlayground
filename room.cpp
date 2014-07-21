#include "room.h"
#include "QPainter"
#include <iostream>
#include "map.h"
#include "QStyle"

Room::Room(QWidget *parent) :
    QWidget(parent)
{

}

void Room::addDoor(QLine doo)
{
    doors.push_back(doo);
}

////Function sets spawn and then checks for the appropriate place to set the spawn
//void Room::setspawn()
//{

//}

////Function sets exit and then checks for the appropriate place to set the exit
//void Room::setexit()
//{

//}

void Room::addconnects(Room *connect)
{
    connects.push_back(connect);
}

void Room::setdifficulty(int dif)
{
    difficulty=dif;
}

int Room::getdifficulty()
{
    return difficulty;
}

std::vector<Room *> Room::getconnects()
{
    return connects;
}

//Report to the parent widget the current details of the room
void Room::mousePressEvent(QMouseEvent *)
{
    QString name=QString::fromStdString("Name : %1. ").arg(objectName());
    QString sizz=QString::fromStdString("Size(H/W) : %1/").arg(size().height());
    QString sizz1=QString::fromStdString("%1. ").arg(size().width());
    QString doorz=QString::fromStdString("Doors : %1. ").arg(doors.size());
    QString diff=QString::fromStdString("Difficulty : %1. ").arg(difficulty);
    ((Map *)parent())->Display(name.append(sizz).append(sizz1).append(doorz).append(diff));
}
