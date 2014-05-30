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
    explicit Room(int,int,QWidget *parent=0);
    ~Room();
    void addDoor(QLine);
    void setspot(bool);
    void addconnects(Room*);

protected:
    void paintEvent(QPaintEvent *e);
private:
    QPolygon poly;
    int centx,centy;
    bool isspawn,issexit;
    std::vector <Room*> connects;
    std::vector <QLine> doors;
};

#endif // ROOM_H
