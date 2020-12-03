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
//#include "newtile.h"
#include "normaltile.h"
#include "longtile.h"
class lane
{
public:
    enum Add_Catagory{Normal, Long, Tile_Undefined};
    enum Judge_result{Good, Bad, Empty, Vaild_Long_Press, Missed_Long_Tile, Continuing_Long_Tile, Undefined};
    lane();
    lane(int keynum, QString);
    void add_to_scene(int id, int total, QGraphicsScene *scene, QRectF bounds);
    Judge_result on_key_pressed(QGraphicsScene *scene, const int sec, const int ms);
    void on_key_released(QGraphicsScene *scene);
    void add_pressed_flash(QGraphicsScene *scene);
    void remove_pressed_flash(QGraphicsScene *scene);
    Judge_result update(QGraphicsScene *scene);
    NewTile* addtile(Add_Catagory, QGraphicsScene*); //should be virtual anyway
    bool is_tile_list_empty();
    int getkeynum();
    void getlastpress_tme(int&, int&);
    int getlongpress_time();
    int getfall_time();
private:

    int id, keynum{Qt::Key_X};
    Add_Catagory add_catagory;
    int key_width{180}, key_height{390};//piano key
    QRectF bound_rect;

    QPixmap piano_key_pix;
    QGraphicsPixmapItem *piano_key_item;
    QString piano_key_name;
    QGraphicsLineItem *left_line, *right_line, *green_indicator;
    int green_height{250}; //tolerance level = height of green area //of normal note...
    int valid_press_height, bottom_height;

    //for glowing effects
    QString glow_name;
    //const int flashtime{10}; //in milliseconds// now i use long press, so it is obsolete
    QGraphicsPixmapItem *glow_flash;

    //for falling tiles
    //const int fall_time = 1200;//if fall time is constant
    const int fall_speed = 2; //if fall speed is constant, now using, however it is useless since i use qpointf instead
    int falling_time;
    QList<NewTile*> tile_list;

    //for longpress
    bool is_long_pressing{false};
    int long_pressing_time{0};
    int lastpress_sec{0}, lastpress_ms{0};
    const int cycles_per_combo{25};

    //for file purpose only
    QString folder_for_all{"c:/orz"};
};

#endif // LANE_H
