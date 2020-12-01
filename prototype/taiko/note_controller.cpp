#include "normal_note.h"
#include "note_controller.h"

#include <QRandomGenerator>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <fstream>
#include <string>
using std::ws;


Note_controller::Note_controller(QObject *parent) : QObject(parent)
{

}

void Note_controller::setScene(QGraphicsScene* scene)
{
    this->scene = scene;
}

void Note_controller::init(QString beatmap_path)
{
    beatmap_path = "F:/testing/1.orz";

    std::ifstream beatmap(beatmap_path.toStdString());

    std::string temp;
    beatmap >> temp;  // ignore title
    beatmap >> temp;  // ignore level

    beatmap >> num_of_notes;

    for(int i = 0; i < num_of_notes ; ++i){
        beatmap >> temp;
        qDebug() << temp.c_str(); // start time
        beatmap >> temp;
        qDebug() << temp.c_str(); // note type
        beatmap >> temp;
        qDebug() << temp.c_str(); // speed
    }

    //    QFile inputFile(beatmap_path);

    //    if (inputFile.open(QIODevice::ReadOnly))
    //    {
    //        QTextStream in(&inputFile);
    //        while (!in.atEnd())
    //        {
    //            QString line = in.readLine();
    //            qDebug() << line;
    //        }
    //        inputFile.close();
    //    }
}

void Note_controller::start()
{
    for(int i = 0 ; i < 10 ; ++i){
        int value = QRandomGenerator::global()->generate()%2;
        //        Note* note = new Normal_note(650 + i * 100, 150 , 130 , 0.5 , Normal_note::note_type::blue_note , this);
        Note* note;
        if(value == 0){
            note = new Normal_note(650 + i * 50, 150 , 130 , 0.5 , Normal_note::note_type::red_note , this);
        }else{
            note = new Normal_note(650 + i * 50, 150 , 130 , 0.5 , Normal_note::note_type::blue_note , this);
        }
        note->init(scene);
        note->start_move();
        note_queue.enqueue(note);
        connect(note, SIGNAL(die()) , this , SLOT(dequeue()));
    }
}

void Note_controller::judge(int drum_or_rim, int performance)
{
    if(!note_queue.isEmpty()){
        Note* note = note_queue.head();
        Normal_note* normal_note = (Normal_note*) note;

        // if it is a normal note
        if(normal_note != nullptr){
            if(normal_note->get_note_type() == drum_or_rim){
                Note* note = note_queue.dequeue();
                note->get_hit(performance);
            }
        }
    }
}

void Note_controller::dequeue()
{
    if(!note_queue.isEmpty()){
        note_queue.dequeue();
    }
}


