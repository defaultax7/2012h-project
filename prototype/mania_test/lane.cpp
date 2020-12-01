#include "lane.h"

lane::lane(): glow_name(":/image/mania_test/images/glowing_top.png")
{

}

void lane::add_to_scene(int id, int total, QGraphicsScene *scene, QRectF bounds){
    int hwidth = key_width/2;
    int xpos = bounds.width()/2 + hwidth*(2*id-total);

    if (id % 2){
        piano_key_name = ":/image/mania_test/images/key_e.png";
    }else{
        piano_key_name = ":/image/mania_test/images/key_d.png";
    }
    this->id = id; //though this proterty is somehow useless...

    piano_key_pix.load(piano_key_name);
    piano_key_pix = piano_key_pix.scaled(QSize(key_width, key_height));

    piano_key_item = new QGraphicsPixmapItem;
    piano_key_item->setPixmap(piano_key_pix);
    scene->addItem(piano_key_item);
    piano_key_item->setOffset(xpos, bounds.height()-key_height);

    QPen whitepen{Qt::white};
    whitepen.setWidth(1);
    left_line = scene->addLine(xpos,0, xpos,bounds.height()-key_height, QPen(Qt::white));
    right_line = scene->addLine(xpos+key_width,0, xpos+key_width,bounds.height()-key_height, QPen(Qt::white));
    //piano_key = QGraphicsPixmapItem(piano_key_name);
    //piano_key = scene->addPixmap(piano_key_pic);
    //piano_key->setOffset(xpos, bounds.height()-195);
    bound = QRectF{(double)xpos,0,(double)key_width,bounds.height()};
}

void lane::play_pressed_flash(QString image_path, QGraphicsScene *scene)
{ //this is most modified from alex's drum_flash, the image path is useless now
    QPixmap glow_pix{glow_name};
    glow_pix = glow_pix.scaled(QSize(key_width,1), Qt::KeepAspectRatioByExpanding);
    QGraphicsPixmapItem glow_flash{glow_pix};
    glow_flash.setOffset(bound.x(), bound.y()+bound.height()-key_height-40); //lazy, the glowing is of aspect ratio 4:1
    scene->addItem(&glow_flash);

    QTime t;
    t.start();
    while (t.elapsed() < flashtime) {
        QCoreApplication::processEvents();
    }
    scene->removeItem(&glow_flash);
}

void lane::on_key_pressed(QGraphicsScene *scene){
    qDebug() << "id is " << id << endl;
    play_pressed_flash("", scene);
}
