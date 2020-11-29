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
    void testing();
private:
    enum hit_type { drum , rim };

    Ui::taiko_window *ui;
    void showEvent(QShowEvent* event);

    QGraphicsScene scene;
    taiko_performance_view p_view;
    QMediaPlayer music_player;
    QTimer* timer;
    unsigned const int drum_flash_time = 80;  // unit ms

    void play_drum_flash(QString image_path, double x , double y);
    void judge_note(hit_type);

    Note_controller note_controller;

    QGraphicsPixmapItem* judge;
    QGraphicsRectItem* perfect_judge;
    QGraphicsRectItem* good_judge;
    QGraphicsRectItem* bad_judge;
    QSoundEffect drum_sound_player;
    QSoundEffect rim_sound_player;
};

#endif // TAIKO_WINDOW_H
