#include "newtile.h"

NewTile::NewTile(QObject *parent) : QObject(parent)
{

}

NewTile::NewTile(QPointF start_point, QPointF end_point, QPointF delta, QObject *parent):
    QObject(parent), start_point(start_point), end_point(end_point), delta(delta)
{
    pos_point = start_point;
}
void NewTile::init(TileType t_type, int width){ //this should not be used since virtual
    tile_type = t_type;
    if (tile_type == TileType::White){
        tile_map.load(":/image/mania_test/images/white_tile.png");
    }else if (tile_type == TileType::Pink){
        tile_map.load(":/image/mania_test/images/pink_tile.png");
    }

    tile_map = tile_map.scaled(width,1, Qt::KeepAspectRatioByExpanding);
    tile_item = new QGraphicsPixmapItem(tile_map);
    //scene->addItem(tile_item);
    tile_item->setOffset(start_point);
}
/*
void NewTile::remove_from_scene(QGraphicsScene *scene){
    scene->removeItem(tile_item);
}
*/
bool NewTile::update_remove(){ //this also should not be used
    pos_point += delta;
    tile_item->setOffset(pos_point);

    if (pos_point.y() >= end_point.y()) { //this is custom made condition
        emit die();
        return true;
    }
    return false;
}
QPointF NewTile::get_position_point(){
    return pos_point;
}

QGraphicsPixmapItem* NewTile::get_pix_item(){
    return tile_item;
}

