#ifndef MAP_H
#define MAP_H

#include <QWidget>
#include "room.h"

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
};

#endif // MAP_H
