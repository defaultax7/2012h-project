#include "note.h"
#include "QDebug"

Note::Note(QObject* parent) : QObject(parent)
{

}

Note::~Note()
{
    delete timer;
    delete image_item;
}

void Note::init(QGraphicsPixmapItem * image , double x , double y){
    set_image_item(image);
    image_item->setPos(x,y);
    timer = new QTimer;
    connect(timer , SIGNAL(timeout()) , this , SLOT(move()));
}

void Note::set_image_item(QGraphicsPixmapItem * image)
{
    image_item = image;
}

void Note::start_move()
{
    timer->start(10);
}

void Note::move()
{
//    image_item->setOffset(image_item->pos().x() - speed , image_item->pos().y());
    image_item->setPos(image_item->pos().x() - speed , image_item->pos().y());
    qDebug() << image_item->pos().x();
    if(image_item->pos().x() < 0 ){
        delete this;
    }
}
