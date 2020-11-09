#include "mainwindow.h"
#include "musicplayer.h"

#include <QApplication>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

    MusicPlayer* p = new MusicPlayer;
    p->show();

    return a.exec();
}
