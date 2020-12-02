#include "note.h"
#include "QDebug"
#include <QGraphicsScene>

Note::Note(double x, double y, double endpoint, double speed, QObject *parent) : QObject(parent) , x(x) , y(y) , speed(speed) , endpoint(endpoint)
{
    timer = new QTimer();
    connect(timer , SIGNAL(timeout()) , this , SLOT(move()));
}

Note::~Note()
{
    delete timer;
    // if note is not put in the scene, image item will be nullptr
    if(image_item != nullptr){
        delete image_item;
    }
}

void Note::start_move()
{
    timer->start(refresh_rate);
}

int Note::getX() const
{
    return x;
}

void Note::stop()
{
    is_stop = true;
}

void Note::unstop(){
    is_stop = false;
}

void Note::move()
{
    if(!is_stop){
        x -= speed;
        image_item->setPos(x , y);
        if(image_item->pos().x() < endpoint ){
            emit die();
            emit note_was_missed();
            delete this;
        }
    }
}
