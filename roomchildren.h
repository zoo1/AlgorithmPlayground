#include "room.h"
#ifndef ROOMCHILDREN_H
#define ROOMCHILDREN_H

class Block : public Room
{
    Q_OBJECT
public:
    explicit Block(int,QWidget *parent=0);
    explicit Block(int, int, QWidget *parent=0);
    void setspawn();
    void setexit();
    ~Block();
protected:
    void paintEvent(QPaintEvent *e);
private:
    QPolygon poly;
};

class Circle : public Room
{
    Q_OBJECT
public:
    explicit Circle(int,QWidget *parent=0);
    void setspawn();
    void setexit();
    ~Circle();
protected:
    void paintEvent(QPaintEvent *e);
private:
    int diameter;
};

class Douhall : public Room
{
    Q_OBJECT
public:
    explicit Douhall(int,int,int,QWidget *parent=0);
    void setspawn();
    void setexit();
    ~Douhall();
protected:
    void paintEvent(QPaintEvent *e);
private:
    bool direction;
    double size;
    int connects;
};

#endif // ROOMCHILDREN_H
