#include "generation.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Generation w;
    w.show();

    return a.exec();
}
