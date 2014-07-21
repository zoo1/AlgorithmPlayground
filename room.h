#ifndef ROOM_H
#define ROOM_H

#include <QWidget>
#include "QPolygon"
#include <vector>


class Room : public QWidget
{
    Q_OBJECT

public:
    explicit Room(QWidget *parent = 0);
    void addDoor(QLine);
    void addconnects(Room*);
    virtual void setspawn()=0;
    virtual void setexit()=0;
    void setdifficulty(int);
    int getdifficulty();
    std::vector <Room*> getconnects();

protected:
    virtual void paintEvent(QPaintEvent *e)=0;
    void mousePressEvent(QMouseEvent *);
    std::vector <Room*> connects;
    std::vector <QLine> doors;
    bool isspawn,issexit;
    int is,difficulty;
};

#endif // ROOM_H
