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
    qDebug() << bound_rect.x() << endl;



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

lane::Judge_result lane::on_key_pressed(QGraphicsScene *scene){
    qDebug() << "id is " << id << endl;
    is_long_pressing = true;
    add_pressed_flash(scene);

    //now judge
    if (tile_list.empty()) return Judge_result::Miss;
    qDebug() << "tile0 at " << tile_list[0]->get_position_point() << endl;
    if (tile_list[0]->get_position_point().y() >= bound_rect.height()-key_height-green_height){
        scene->removeItem(tile_list[0]->get_pix_item());
        tile_list.removeFirst();
        return Judge_result::Good;
    }else{
        return Judge_result::Miss;
    }
    return Judge_result::Judge_Undefined;
}

void lane::on_key_released(QGraphicsScene *scene){
    is_long_pressing = false;
    remove_pressed_flash(scene);
}

void lane::update(QGraphicsScene *scene){
    bool to_remove;
    if (tile_list.size() == 0) return;

    to_remove = tile_list[0]->update_remove();
    for (int i=1; i<tile_list.size(); ++i){
        tile_list[i]->update_remove();
    }
    if (to_remove){
        scene->removeItem(tile_list[0]->get_pix_item());
        //tile_list[0]->remove_from_scene(scene);
        tile_list.removeFirst();
    }
}

NewTile* lane::addtile(Tile_Catagory category, QGraphicsScene* scene){
    QPointF st = bound_rect.topLeft() ;
    QPointF ed = bound_rect.bottomLeft() - QPointF{0,(double)key_height};
    QPointF delta{0,2};

    tile_catagory = category;
    if (category == Tile_Catagory::Normal){
        NewTile *temptile = new NormalTile(st,ed,delta);
        if (id % 2) temptile->init(NewTile::TileType::Pink, key_width);
        else temptile->init(NewTile::TileType::White, key_width);
        scene->addItem(temptile->get_pix_item());
        tile_list.append(temptile);
        return temptile;
    }
    return nullptr;
}

