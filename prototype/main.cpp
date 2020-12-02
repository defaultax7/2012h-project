#include "mainwindow.h"


#include <QApplication>

#include <taiko/taiko_window.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    taiko_window w;
    w.show();
    return a.exec();
}
