#ifndef MY_PLAYER_H
#define MY_PLAYER_H

#include <QLinkedList>
#include <QMediaPlayer>

class my_player
{
public:
    my_player();
    void setMuted(bool);
    void add_song(QString);
    void shuffle_song_list();

private:
    QLinkedList<QString>::iterator current_song;
    QMediaPlayer* player;
    QLinkedList<QString> song_list;
};

#endif // MY_PLAYER_H
