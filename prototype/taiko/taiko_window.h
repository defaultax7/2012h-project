#ifndef TAIKO_WINDOW_H
#define TAIKO_WINDOW_H

#include "note.h"
#include "note_controller.h"
#include "score.h"
#include "taiko_performance_view.h"

#include <QMainWindow>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QMediaPlayer>
#include <QTimer>
#include <QSoundEffect>

namespace Ui {
class taiko_window;
}

class taiko_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit taiko_window(QWidget *parent = nullptr);
    ~taiko_window();

    void keyPressEvent(QKeyEvent *event);

private slots:
    void start_game();
    void duration_change(qint64);
    void current_time_change(qint64);

    void on_btn_resume_clicked();

    void on_btn_retry_clicked();

    void on_btn_exit_clicked();

private:
    enum hit_type { drum , rim };

    Ui::taiko_window *ui;

    QGraphicsScene scene;
    taiko_performance_view p_view;
    QMediaPlayer music_player;
    QTimer* timer;
    QGraphicsPixmapItem* judge;
    QSoundEffect drum_sound_player;
    QSoundEffect rim_sound_player;
    QWidget* parent;
    Note_controller note_controller;
    int pause_time;
    unsigned const int drum_flash_time = 80;  // the time that drum flash lasting for ( unit ms )

    void showEvent(QShowEvent* event);
    void play_drum_flash(QString image_path, double x , double y);
    void show_result();
    void pause();
    void resume();
    void retry();
    void hide_pause_screen();
    void show_pause_screen();


};

#endif // TAIKO_WINDOW_H
