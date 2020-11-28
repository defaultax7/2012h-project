#include "normal_note.h"

#include <QGraphicsScene>

Normal_note::Normal_note(double x, double y, double endpoint, double speed, note_type type, QObject* parent) : Note(x, y, endpoint, speed, parent) , type(type)
{
}

void Normal_note::init(QGraphicsScene& scene){
    if(type == note_type::red_note){
        image_item = scene.addPixmap(QPixmap(":/image/image/red_note.png"));
    }else{
        image_item = scene.addPixmap(QPixmap(":/image/image/blue_note.png"));
    }
    image_item->setPos(x,y);
}

void Normal_note::self_destruct()
{
    delete this;
}


void Normal_note::get_hit(){
    image_item->setPixmap(QPixmap(":/image/image/perfect.png"));
    speed = 0;

    connect(timer, SIGNAL(timeout()), this, SLOT(self_destruct()));
    timer->start(afterlife_time);

//    delete this;
}



