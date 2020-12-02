#include "normaltile.h"

NormalTile::NormalTile(QPointF start_point, QPointF end_point, QPointF delta, QObject *parent):
    NewTile(start_point, end_point, delta, parent)
{

}

void NormalTile::init(TileType t_type, int width){
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

bool NormalTile::update_remove(){
    pos_point += delta;
    tile_item->setOffset(pos_point);

    if (pos_point.y() >= end_point.y()) { //this is custom made condition
        emit die();
        return true;
    }
    return false;
}
