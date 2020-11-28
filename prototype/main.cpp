#include "mainwindow.h"
#include "taiko_window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
    taiko_window w;
    w.show();
    return a.exec();
}
