#ifndef LANE_H
#define LANE_H

#include <QGraphicsScene>
#include <QRectF>
#include <QGraphicsLineItem>
#include <QGraphicsPixmapItem>
#include <QTime>
#include <QCoreApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QGraphicsScene>
//#include <mania_test/tile.h>
//#include <mania_test/normal_tile.h>
#include "mania_test/newtile.h"
#include "normaltile.h"
class lane
{
public:
    enum Tile_Catagory{Normal, Long, Tile_Undefined};
    enum Judge_result{Good, Bad, Miss, Vaild_Long_Press, Judge_Undefined};
    lane();
    lane(int keynum);
    void add_to_scene(int id, int total, QGraphicsScene *scene, QRectF bounds);
    Judge_result on_key_pressed(QGraphicsScene *scene);
    void on_key_released(QGraphicsScene *scene);
    void add_pressed_flash(QGraphicsScene *scene);
    void remove_pressed_flash(QGraphicsScene *scene);
    void update(QGraphicsScene *scene);
    NewTile* addtile(Tile_Catagory, QGraphicsScene*); //should be virtual anyway
    int getkeynum();
private:

    int id, keynum{Qt::Key_X};
    Tile_Catagory tile_catagory;
    int key_width{180}, key_height{390};
    bool is_long_pressing{false};
    QRectF bound_rect;

    QPixmap piano_key_pix;
    QGraphicsPixmapItem *piano_key_item;
    QString piano_key_name;
    QGraphicsLineItem *left_line, *right_line, *green_indicator;
    int green_height{250}; //tolerance level = height of green area
    //for glowing effects
    const QString glow_name;
    const int flashtime{10}; //in milliseconds// now i use long press, so it is obsolete
    QGraphicsPixmapItem *glow_flash;

    //for falling tiles
    const int fall_time = 1200;//if fall time is constant
    const int fall_speed = 2; //if fall speed is constant, now using
    //tile *thetiles[100];
    //NewTile *thetiles[100];
    QList<NewTile*> tile_list;
};

#endif // LANE_H
