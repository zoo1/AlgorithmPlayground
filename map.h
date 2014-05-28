#ifndef MAP_H
#define MAP_H

#include <QWidget>

class Map : public QWidget
{
    Q_OBJECT

public:
    explicit Map(QWidget *parent = 0);
    ~Map();
private slots:
    void stage1();
    void stage2();
    void stage3();
};

#endif // MAP_H
