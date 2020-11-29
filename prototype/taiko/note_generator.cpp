#include "normal_note.h"
#include "note_generator.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <fstream>
#include <string>
using std::ws;


Note_generator::Note_generator(QObject *parent) : QObject(parent)
{

}

void Note_generator::setScene(QGraphicsScene* scene)
{
    this->scene = scene;
}

void Note_generator::init(QString beatmap_path)
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

void Note_generator::start()
{
    Note* note = new Normal_note(600 , 150 , 130 , 0.5 , Normal_note::note_type::blue_note , this);
    note->init(scene);
    note->start_move();
    note_queue.enqueue(note);
    connect(note, SIGNAL(die()) , this , SLOT(dequeue()));
}

void Note_generator::judge(int performance)
{
    if(!note_queue.isEmpty()){
        Note* note = note_queue.dequeue();
        note->get_hit();
    }
}

void Note_generator::dequeue()
{
    if(!note_queue.isEmpty()){
        Note* note = note_queue.dequeue();
    }
}


