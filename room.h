#ifndef ROOM_H
#define ROOM_H

#include <QWidget>

namespace Ui {
class Room;
}

class Room : public QWidget
{
    Q_OBJECT

public:
    explicit Room(QWidget *parent = 0);
    ~Room();

private:
    Ui::Room *ui;
};

#endif // ROOM_H
