#include "mainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


//TODO: Maintain two lists for all the positions of the black and white pieces to optimize functions that require looking up the pieces on the board
