#include "normal_note.h"

#include <QGraphicsScene>

Normal_note::Normal_note(double x, double y, double endpoint, double speed, normal_note_type type, QObject* parent) : Note(x, y, endpoint, speed, parent) , type(type)
{
}

void Normal_note::init(QGraphicsScene* scene){
    if(type == normal_note_type::red_note){
        image_item = scene->addPixmap(QPixmap(":/image/image/red_note.png"));
    }else{
        image_item = scene->addPixmap(QPixmap(":/image/image/blue_note.png"));
    }
    image_item->setPos(x,y);
}

Normal_note::normal_note_type Normal_note::get_note_type() const
{
    return type;
}

void Normal_note::self_destruct()
{
    delete this;
}


void Normal_note::get_hit(int performance){

    // perfect
    if(performance == 0){
        image_item->setPixmap(QPixmap(":/image/image/perfect.png"));
    }else if(performance == 1){ // good
        image_item->setPixmap(QPixmap(":/image/image/good.png"));
    }
    speed = 0;

    image_item->setOpacity(1);

    emit note_was_hit(performance);

    // kill itselft after given amount of time (afterlife_time)
    connect(timer, SIGNAL(timeout()), this, SLOT(self_destruct()));
    timer->start(afterlife_time);

}



