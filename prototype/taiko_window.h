#ifndef TAIKO_WINDOW_H
#define TAIKO_WINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QGraphicsScene>

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

private:
    Ui::taiko_window *ui;
    void showEvent(QShowEvent* event);

    QGraphicsScene scene;
};

#endif // TAIKO_WINDOW_H
