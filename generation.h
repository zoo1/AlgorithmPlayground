#ifndef GENERATION_H
#define GENERATION_H

#include <QMainWindow>
#include "map.h"
namespace Ui {
class Generation;
}

class Generation : public QMainWindow
{
    Q_OBJECT

public:
    explicit Generation(QWidget *parent = 0);
    ~Generation();

private slots:
    void on_pushButton_clicked();
    void on_Roommin_textChanged();
    void on_Roommax_textChanged();
    void on_checkBox_clicked(bool checked);
    void on_checkBox_2_clicked(bool checked);
    void on_checkBox_3_clicked(bool checked);
    void on_checkBox_4_clicked(bool checked);

private:
    Ui::Generation *ui;
    Map *mymap;
    void checkminmax();
    void StartGeneration();
    int convert_value(QString);
};

#endif // GENERATION_H
