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
    //enum Tile_Catagory{Normal, Long, Tile_Undefined};
    enum TileType{White, Pink, Mario, Undefined};
    NewTile(QObject *parent = nullptr);
    NewTile(QPointF start_point, QPointF end_point, QPointF delta, QObject *parent=nullptr);
    virtual void init(TileType, int width);
    //void remove_from_scene();
    virtual bool update_remove();
    QPointF get_position_point();
    QGraphicsPixmapItem* get_pix_item();
signals:
    void die();
protected:
    QPointF start_point, pos_point, end_point, delta;
    QPixmap tile_map;
    QGraphicsPixmapItem *tile_item;
    TileType tile_type{TileType::Undefined};

    QTimer* timer;
    int refresh_rate{2}; //not changed afterwards
};

#endif // NEWTILE_H
