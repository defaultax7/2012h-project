#include "longtile.h"

LongTile::LongTile(QPointF start_point, QPointF end_point, QPointF delta,TileType t_type,QString folder, int length, QObject *parent):
    NewTile(start_point, end_point, delta, Long,folder, parent), tile_type(t_type), length(length)
{
    //length = delta
    tail_point = start_point - QPointF(0,length);
}

void LongTile::init(int width){
    this->width = width;
    if (tile_type == TileType::White){
        tile_map.load(folder_for_all+"/images/white_tile.png");
        //":/image/mania_test/images/white_tile.png"
    }else if (tile_type == TileType::Pink){
        tile_map.load(folder_for_all+"/images/pink_tile.png");
    }
    //pos_point += 10*delta;
    //tail_point += 10*delta;

    QPointF offset_point = tail_point;
    //if (tail_point.y() < 0) offset_point.setY(0);

    tile_map = tile_map.scaled(width,length, Qt::IgnoreAspectRatio);
    tile_item = new QGraphicsPixmapItem(tile_map);
    //scene->addItem(tile_item);
    tile_item->setOffset(offset_point);
}

bool LongTile::update_remove(double actual_cycle){
    pos_point += delta*actual_cycle;
    tail_point += delta*actual_cycle;

    QPointF offset_point = tail_point;
    //if (tail_point.y() < 0) offset_point.setY(0);
    tile_item->setOffset(offset_point);

    /*
    int height = pos_point.y()-offset_point.y();
    if (pos_point.y() > end_point.y()) height = end_point.y()-offset_point.y();

    temp_map = tile_map.scaled(width, height, Qt::IgnoreAspectRatio);
    //delete temp_tile_item; //em first need to do this
    temp_tile_item = new QGraphicsPixmapItem(temp_map);
    temp_tile_item->setOffset(offset_point);
    */
    if (offset_point.y() >= end_point.y()) { //this is custom made condition
        emit die();
        return true;
    }


    return false;
}

void LongTile::set_ispresed(bool is_pressed){
    ispressed = is_pressed;
}
bool LongTile::get_ispressed(){
    return ispressed;
}

const QPointF LongTile::get_tail_point(){
    return tail_point;
}
