#ifndef MAP_H
#define MAP_H

#include <QWidget>
#include "room.h"
#include "hallway.h"

//structure for comparing multiple rooms, used in the minimum spanning tree algorithm
struct connect {
  Room* r1;
  Room* r2;
};
int compareconnect(const void * a, const void * b);

class Map : public QWidget
{
    Q_OBJECT

public:
    explicit Map(QWidget *parent = 0);
    void Display(QString);
    ~Map();
private slots:
    void stage1();
    void stage2();
    void stage3();
    void stage4();
private:
    void createhallways(Room*, Room*);
    void recursiveDif(std::vector<Room *>,int);
    bool isinside(Room*,QPoint);
};

#endif // MAP_H
