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
private:
    void createhallways(Room*, Room*);
};

#endif // MAP_H
