#include "lane.h"

lane::lane() = default;
lane::lane(int keynum):
    keynum(keynum),
    glow_name(":/image/mania_test/images/glowing_top.png")
{
}

int lane::getkeynum(){
    return keynum;
}

void lane::add_to_scene(int id, int total, QGraphicsScene *scene, QRectF bounds){
    int hwidth = key_width/2;
    int xpos = bounds.width()/2 + hwidth*(2*id-total);

    if (id == 0){ //now brute force, but should use srting instead...
        piano_key_name = ":/image/mania_test/images/key_d.png";
    }else if (id == 1){
        piano_key_name = ":/image/mania_test/images/key_e.png";
    }else if (id == 2){
        piano_key_name = ":/image/mania_test/images/key_s.png";
    }else{
        piano_key_name = ":/image/mania_test/images/key_p.png";
    }

    this->id = id; //though this proterty is somehow useless...

    piano_key_pix.load(piano_key_name);
    piano_key_pix = piano_key_pix.scaled(QSize(key_width, key_height));

    piano_key_item = new QGraphicsPixmapItem;
    piano_key_item->setPixmap(piano_key_pix);
    scene->addItem(piano_key_item);
    piano_key_item->setOffset(xpos, bounds.height()-key_height);
    piano_key_item->setZValue(100); //nothing can stop me

    QPen whitepen{Qt::white};
    whitepen.setWidth(1);
    QPen greenpen{Qt::green};
    whitepen.setWidth(2);
    left_line = scene->addLine(xpos,0, xpos,bounds.height()-key_height, whitepen);
    right_line = scene->addLine(xpos+key_width,0, xpos+key_width,bounds.height()-key_height, whitepen);

    int green_line_height = bounds.height()-key_height-green_height; //tailor made
    green_indicator = scene->addLine(xpos, green_line_height, xpos+key_width, green_line_height, greenpen);
    //piano_key = QGraphicsPixmapItem(piano_key_name);
    //piano_key = scene->addPixmap(piano_key_pic);
    //piano_key->setOffset(xpos, bounds.height()-195);
    bound_rect = QRectF{(double)xpos,0, (double)key_width,bounds.height()};
    valid_press_height = bound_rect.height()-key_height-green_height;
    bottom_height = bound_rect.height()-key_height;
    //qDebug() << bound_rect.x() << endl;
    falling_time = bottom_height/2; //here i hardcode this...


    //thetiles[0] = new normal_tile(st,ed, normal_tile::Tiletype::Pink);
    //thetiles[0]->init(scene, key_width);
    //thetiles[0]->print_delta();
    //thetiles[0]->start_move();


}

void lane::add_pressed_flash(QGraphicsScene *scene)
{ //this is most modified from alex's drum_flash, the image path is useless now
    QPixmap glow_pix{glow_name};
    glow_pix = glow_pix.scaled(QSize(key_width,1), Qt::KeepAspectRatioByExpanding);
    glow_flash = new QGraphicsPixmapItem{glow_pix};
    glow_flash->setZValue(2); //higher than tile i think
    glow_flash->setOffset(bound_rect.x(), bound_rect.y()+bound_rect.height()-key_height-40); //lazy, the glowing is of aspect ratio 4:1
    scene->addItem(glow_flash);

    /*
    QTime t;
    t.start();
    while (t.elapsed() < flashtime) {
        QCoreApplication::processEvents();
    }
    scene->removeItem(&glow_flash);
    */
}
void lane::remove_pressed_flash(QGraphicsScene *scene){
    scene->removeItem(glow_flash);
}

lane::Judge_result lane::on_key_pressed(QGraphicsScene *scene, const int sec, const int ms){
    qDebug() << "id is " << id << endl;
    //is_long_pressing = true;
    is_long_pressing = true; //now i need this for creator...
    lastpress_sec = sec;
    lastpress_ms = ms;
    add_pressed_flash(scene);

    //now judge
    if (tile_list.empty()) return Judge_result::Empty;
    NewTile *toptile = tile_list[0];
    qDebug() << "tile0 at " << tile_list[0]->get_position_point() << endl;
    if (tile_list[0]->get_position_point().y() >= bound_rect.height()-key_height-green_height){
        if (toptile->get_tile_catagory() == NewTile::Tile_Catagory::Normal){
            scene->removeItem(tile_list[0]->get_pix_item());
            tile_list.removeFirst();
            return Judge_result::Good;
        }
        else if (toptile->get_tile_catagory() == NewTile::Tile_Catagory::Long){
            LongTile* long_ptr = dynamic_cast<LongTile*>(toptile);
            long_ptr->set_ispresed(true);
            //previously set long_pressing here
            return Judge_result::Vaild_Long_Press;
        }

    }else{
        return Judge_result::Empty;
    }
    return Judge_result::Undefined;
}

void lane::on_key_released(QGraphicsScene *scene){
    is_long_pressing = false;
    long_pressing_time = 0;
    remove_pressed_flash(scene);

    if (tile_list.empty()) return;
    NewTile *toptile = tile_list[0];
    if (toptile->get_tile_catagory() == NewTile::Tile_Catagory::Long){ //special handler of long tile
        LongTile* long_ptr = dynamic_cast<LongTile*>(toptile);
        long_ptr->set_ispresed(false);
    }

}

lane::Judge_result lane::update(QGraphicsScene *scene){
    bool to_remove;
    lane::Judge_result response = Judge_result::Empty; //default response

    if (is_long_pressing) long_pressing_time++;
    //long_pressing_time++;
    if (tile_list.empty()) return Judge_result::Empty;

    for (int i=1; i<tile_list.size(); ++i){
        tile_list[i]->update_remove();
    }
    to_remove = tile_list[0]->update_remove(); //special treatment of bottom tile
    if (tile_list[0]->get_tile_catagory() == NewTile::Tile_Catagory::Long){
        LongTile* long_ptr = dynamic_cast<LongTile*>(tile_list[0]);
        if (long_ptr->get_position_point().y() >= bottom_height && long_ptr->get_tail_point().y() < valid_press_height && long_ptr->get_ispressed() == false){
            response = Judge_result::Missed_Long_Tile;
        }
        else if (long_ptr->get_ispressed() && long_pressing_time % cycles_per_combo == 0){ //avoid adding so many combo, hardcode
            response = Judge_result::Continuing_Long_Tile;
        }
    }
    for (int i=0; i<tile_list.size(); ++i){
        if (tile_list[i]->get_tile_catagory() == NewTile::Tile_Catagory::Long){
            scene->removeItem(tile_list[i]->get_pix_item());
            tile_list[i]->update_pix_item();
            scene->addItem(tile_list[i]->get_pix_item());
        }
    }
    if (to_remove){
        scene->removeItem(tile_list[0]->get_pix_item());
        //tile_list[0]->remove_from_scene(scene);
        tile_list.removeFirst();
    }
    return response;
}

NewTile* lane::addtile(Add_Catagory category, QGraphicsScene* scene){
    QPointF st = bound_rect.topLeft() ;
    QPointF ed = bound_rect.bottomLeft() - QPointF{0,(double)key_height};
    QPointF delta{0,2};

    add_catagory = category;
    if (category == Add_Catagory::Normal){
        NewTile *temptile{nullptr};
        if (id % 2){
            temptile = new NormalTile(st,ed,delta, NormalTile::Pink);
        }else{
            temptile = new NormalTile(st,ed,delta, NormalTile::White);
        }
        temptile->init(key_width);
        //if (id % 2) temptile->init(NewTile::TileType::Pink, key_width);
        //else temptile->init(NewTile::TileType::White, key_width);
        scene->addItem(temptile->get_pix_item());
        tile_list.append(temptile);
        return temptile;
    }
    else if (category == Add_Catagory::Long){
        NewTile *temptile{nullptr};
        if (id % 2){
            temptile = new LongTile(st,ed,delta, LongTile::Pink); //length not specified, so 1000
        }else{
            temptile = new LongTile(st,ed,delta, LongTile::White);
        }
        temptile->init(key_width);
        scene->addItem(temptile->get_pix_item());
        tile_list.append(temptile);
        return temptile;
    }
    return nullptr;
}

bool lane::is_tile_list_empty(){
    return tile_list.empty();
}

int lane::getlongpress_time(){
    return long_pressing_time;
}

void lane::getlastpress_tme(int &sec, int &ms){
    sec = lastpress_sec;
    ms = lastpress_ms;
}

int lane::getfall_time(){
    return falling_time;
}

