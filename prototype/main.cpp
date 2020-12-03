#include "mainwindow.h"


#include <QApplication>

#include <taiko/map/map_selection_window.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    map_selection_window w;
    w.show();
    return a.exec();
}
