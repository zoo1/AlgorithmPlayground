#ifndef HALLWAY_H
#define HALLWAY_H

#include <QWidget>
#include "QLine"
#include <vector>


class Hallway : public QWidget
{
    Q_OBJECT

public:
    explicit Hallway(QWidget *parent = 0);
    explicit Hallway(int,int,bool,QWidget *parent=0);
    ~Hallway();
    void addDoor(QLine);

protected:
    void paintEvent(QPaintEvent *e);
private:
    QLine wall1,wall2;
    int centx,centy;
    std::vector <QLine> doors;
};

#endif // HALLWAY_H
