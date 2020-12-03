#include "newtile.h"

NewTile::NewTile(QObject *parent) : QObject(parent) //useless
{

}

NewTile::NewTile(QPointF start_point, QPointF end_point, QPointF delta,Tile_Catagory t_cata,QString folder, QObject *parent):
    QObject(parent), start_point(start_point), end_point(end_point), delta(delta), tile_catagory(t_cata), folder_for_all(folder)
{
    pos_point = start_point;
}
void NewTile::init( int width){ //this should not be used since virtual
}

bool NewTile::update_remove(double actual_cycle){ //this also should not be used
    /*
    pos_point += delta;
    tile_item->setOffset(pos_point);

    if (pos_point.y() >= end_point.y()) { //this is custom made condition
        emit die();
        return true;
    }
    return false;
    */
    return false;
}
QPointF NewTile::get_position_point(){
    return pos_point;
}

QGraphicsPixmapItem* NewTile::get_pix_item(){
    return tile_item;
}

void NewTile::update_pix_item(){ //it is useless now
    delete tile_item;
    tile_item = temp_tile_item;
}

NewTile::Tile_Catagory NewTile::get_tile_catagory(){
    return tile_catagory;
}

