#ifndef MUSIC_PLAYER_WINDOW_H
#define MUSIC_PLAYER_WINDOW_H

#include "my_player.h"

#include <QMainWindow>
#include <QMediaPlayer>
#include <QTreeWidget>

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

    void on_btn_delete_all_clicked();

    void on_btn_start_clicked();

    void on_btn_stop_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_btn_next_clicked();

    void on_btn_previous_clicked();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void update_duration(qint64);

    void update_current_time(qint64);

    void update_current_song(QString);

    void on_prograss_bar_sliderReleased();

    void start_button_update(QMediaPlayer::State);

    void auto_next_song();

    void on_prograss_bar_valueChanged(int value);

private:
    Ui::music_player_window *ui;
    my_player player;
    bool is_mute = false;

    void closeEvent(QCloseEvent *);
    QString number_to_timestring(qint64);
};

#endif // MUSIC_PLAYER_WINDOW_H
