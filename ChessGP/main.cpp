#include "mainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


//TODO
//movePiece substitute for check-operations

//another thing we can do to fasten up the look - 
//ups is to maintain two arrays of all the positions of the black and white pieces
