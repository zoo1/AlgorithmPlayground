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
    void setspawn();
    void setexit();
    void addconnects(Room*);
    void setdifficulty(int);
    int getdifficulty();
    std::vector <Room*> getconnects();

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *);
private:
    QPolygon poly;
    bool isspawn,issexit;
    int is,difficulty;
    std::vector <Room*> connects;
    std::vector <QLine> doors;
};

#endif // ROOM_H
