#include "mainwindow.h"


#include <QApplication>

#include <taiko/taiko_window.h>

#include <music_player/music_player_window.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
    taiko_window w;
//    music_player_window w;
    w.show();
    return a.exec();
}
