#ifndef NEWTILE_H
#define NEWTILE_H

#include <QObject>
#include <QPointF>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QTimer> //i suppose it is ok...
#include <QDebug>
class NewTile : public QObject
{
    Q_OBJECT
public:
    enum Tile_Catagory{Normal, Long, Undefined};

    NewTile(QObject *parent = nullptr);
    //virtual ~NewTile();
    NewTile(QPointF start_point, QPointF end_point, QPointF delta,Tile_Catagory, QObject *parent=nullptr);
    virtual void init(int width);
    //void remove_from_scene();
    virtual bool update_remove();
    QPointF get_position_point();
    QGraphicsPixmapItem* get_pix_item();
    void update_pix_item();
    Tile_Catagory get_tile_catagory();
signals:
    void die();
protected:
    QPointF start_point, pos_point, end_point, delta;
    QPixmap tile_map, temp_map;
    QGraphicsPixmapItem *tile_item, *temp_tile_item;
    Tile_Catagory tile_catagory;

    QTimer* timer;
    int refresh_rate{2}; //not changed afterwards
};

#endif // NEWTILE_H
