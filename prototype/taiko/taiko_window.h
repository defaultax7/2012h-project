#ifndef TAIKO_WINDOW_H
#define TAIKO_WINDOW_H

#include "note.h"
#include "score.h"
#include "taiko_performance_view.h"

#include <QMainWindow>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QMediaPlayer>
#include <QTimer>

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
    Ui::taiko_window *ui;
    void showEvent(QShowEvent* event);

    QGraphicsScene scene;
    taiko_performance_view p_view;
    QMediaPlayer music_player;
    QTimer* timer;

    Note* note;  // use for testing
};

#endif // TAIKO_WINDOW_H
