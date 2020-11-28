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
    delete image_item;
}

void Note::start_move()
{
    timer->start(refresh_rate);
}

void Note::move()
{
    image_item->setPos(image_item->pos().x() - speed , image_item->pos().y());
    if(image_item->pos().x() < endpoint ){
        delete this;
    }
}
