#ifndef MANIA_WINDOW_H
#define MANIA_WINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QRectF>
#include <QKeyEvent>
#include "lane.h"

namespace Ui {
class mania_window;
}

class mania_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit mania_window(QWidget *parent = nullptr);
    ~mania_window();

private slots:
    void ui_view_clicked(int xpos, int ypos);

    void on_pushButton_clicked();
    void keyPressEvent(QKeyEvent *event);
private:
    Ui::mania_window *ui;
    QGraphicsScene thescene{0,0,0,0};
    QGraphicsPixmapItem *localitems[100]; //points to temp local items
    //they should be unders ojects anyway
    QRectF bound_rect{0,0,400,600};
    QGraphicsRectItem *bound_rect_item; //a pointer to bounding rect, but should beuseless
    //everything should stay within this rect, aspect ratio should be 600:400 anyway

    int game_stage{0};
    QString images[100]; //should not be changed...

    int num_lanes{4};
    lane thelanes[100];

    int lanekeys[100];
};

#endif // MANIA_WINDOW_H
