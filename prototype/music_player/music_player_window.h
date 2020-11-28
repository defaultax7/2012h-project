#ifndef MUSIC_PLAYER_WINDOW_H
#define MUSIC_PLAYER_WINDOW_H

#include "my_player.h"

#include <QMainWindow>
#include <QMediaPlayer>

namespace Ui {
class music_player_window;
}

class music_player_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit music_player_window(QWidget *parent = nullptr);
    ~music_player_window();

private slots:
    void on_search_song_textChanged(const QString &arg1);

    void on_btn_mute_clicked();

    void on_btn_open_music_clicked();

    void update_song_list(QLinkedList<QString>);

    void on_btn_delete_clicked();

    void on_btn_open_folder_clicked();

    void on_btn_random_song_list_clicked();

private:
    Ui::music_player_window *ui;
    my_player player;
    bool is_mute = false;

    void closeEvent(QCloseEvent *);
};

#endif // MUSIC_PLAYER_WINDOW_H
