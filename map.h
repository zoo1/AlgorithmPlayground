#ifndef MAP_H
#define MAP_H

#include <QWidget>

class Map : public QWidget
{
    Q_OBJECT

public:
    explicit Map(QWidget *parent = 0);
    ~Map();

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // MAP_H
