#include "normaltile.h"

NormalTile::NormalTile(QPointF start_point, QPointF end_point, QPointF delta,TileType t_type,QString folder, QObject *parent):
    NewTile(start_point, end_point, delta, Tile_Catagory::Normal, folder, parent), tile_type(t_type)
{
}

void NormalTile::init(int width){
    if (tile_type == TileType::White){
        //tile_map.load(":/image/mania_test/images/white_tile.png");
        tile_map.load(folder_for_all + "/images/white_tile.png");
    }else if (tile_type == TileType::Pink){
        tile_map.load(folder_for_all + "/images/pink_tile.png");
    }

    tile_map = tile_map.scaled(width,1, Qt::KeepAspectRatioByExpanding);
    tile_item = new QGraphicsPixmapItem(tile_map);
    //scene->addItem(tile_item);
    tile_item->setOffset(start_point-QPointF(0,20)); //tailor
}

bool NormalTile::update_remove(double actual_cycle){
    pos_point += delta*actual_cycle;
    tile_item->setOffset(pos_point);

    if (pos_point.y() >= end_point.y()) { //this is custom made condition
        emit die();
        return true;
    }
    return false;
}
