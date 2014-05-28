#ifndef ROOM_H
#define ROOM_H

#include <QWidget>
#include "QPolygon"

class Room : public QWidget
{
    Q_OBJECT

public:
    explicit Room(QWidget *parent = 0);
    explicit Room(int,int,QWidget *parent=0);
    ~Room();
    void translate(int,int);
    int ceny();
    int cenx();

protected:
    void paintEvent(QPaintEvent *e);
private:
    QPolygon poly;
    int centx,centy;
};

#endif // ROOM_H
