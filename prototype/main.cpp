#include "mainwindow.h"


#include <QApplication>

#include <taiko/map_selection_window.h>
#include <taiko/taiko_window.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    taiko_window w;
    map_selection_window w;
    w.show();
    return a.exec();
}
