#include "mainwindow.h"


#include <QApplication>

#include <taiko/taiko_window.h>

#include <music_player/music_player_window.h>

#include <mania_test/mania_window.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    taiko_window w;
    //music_player_window w;
    mania_window w; //lets see
    w.show();
    return a.exec();
}
