#include "room.h"
#include "ui_room.h"

Room::Room(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Room)
{
    ui->setupUi(this);
}

Room::~Room()
{
    delete ui;
}
