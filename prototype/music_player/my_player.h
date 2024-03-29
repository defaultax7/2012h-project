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
    QLinkedList<QString> get_filtered_song_list(const QString&);
    QLinkedList<QString> get_song_list();
    void jump_to(qint64);
    QMediaPlayer::State get_state() const;
    void pause();
    void play();

signals:
    void song_list_changed(QLinkedList<QString>);
    void duration_update(qint64);
    void current_time_update(qint64);
    void song_update(QString);
    void update_start_button(QMediaPlayer::State);
    void auto_next_song();

private slots:
    void duration_change(qint64);
    void current_time_change(qint64);
    void player_state_change(QMediaPlayer::State);

private:
    bool pointing_to_song = false;
    QLinkedList<QString>::iterator current_song;
    QMediaPlayer* player;
    QLinkedList<QString> song_list;

    void reset_current_song();
};

#endif // MY_PLAYER_H
