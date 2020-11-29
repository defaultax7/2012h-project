#ifndef MY_PLAYER_H
#define MY_PLAYER_H

#include <QLinkedList>
#include <QMediaPlayer>

class my_player : public QObject
{
    Q_OBJECT
public:
    my_player();
    void setMuted(bool);
    void add_song(QString);
    void add_songs(QStringList);
    void remove_song(QString);
    void shuffle_song_list();
    void remove_all();
    void play_song(QString song_name);
    void stop();
    void change_volume(int volume);
    void next();
    void prev();

signals:
    void song_list_changed(QLinkedList<QString>);

private:
    QLinkedList<QString>::iterator current_song;
    QMediaPlayer* player;
    QLinkedList<QString> song_list;
};

#endif // MY_PLAYER_H
