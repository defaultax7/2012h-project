#ifndef LANE_H
#define LANE_H

#include <QGraphicsScene>
#include <QRectF>
#include <QGraphicsLineItem>
#include <QGraphicsPixmapItem>
#include <QTime>
#include <QCoreApplication>
#include <QDebug>
class lane
{
public:
    lane();
    void add_to_scene(int id, int total, QGraphicsScene *scene, QRectF bounds);
    void on_key_pressed(QGraphicsScene *scene);
    void play_pressed_flash(QString image_path, QGraphicsScene *scene);
private:
    int id;
    int key_width{180}, key_height{390};
    QRectF bound;

    QPixmap piano_key_pix;
    QGraphicsPixmapItem *piano_key_item;
    QString piano_key_name;
    QGraphicsLineItem *left_line, *right_line;

    //for glowing effects
    const QString glow_name;
    const int flashtime{50}; //in milliseconds
};

#endif // LANE_H
