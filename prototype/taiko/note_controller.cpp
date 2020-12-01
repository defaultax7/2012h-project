#include "normal_note.h"
#include "note_controller.h"

#include <QRandomGenerator>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <fstream>
#include <string>
#include <QTimer>
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
    //    beatmap_path = "F:/testing/1.orz";

    //    std::ifstream beatmap(beatmap_path.toStdString());

    //    std::string temp;
    //    beatmap >> temp;  // ignore title
    //    beatmap >> temp;  // ignore level

    //    beatmap >> num_of_notes;

    //    notes = new Note*[num_of_notes];
    //    for(int i = 0; i < num_of_notes ; ++i){
    //        notes[i] = new Normal_note(650 + i * 50, 150 , 130 , 0.5 , Normal_note::note_type::red_note , this);
    //        beatmap >> temp;
    //        qDebug() << temp.c_str(); // start time
    //        beatmap >> temp;
    //        qDebug() << temp.c_str(); // note type
    //        beatmap >> temp;
    //        qDebug() << temp.c_str(); // speed
    //    }

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

    std::ifstream beatmap("F:/testing/1.orz");

    std::string temp;
    beatmap >> temp;  // ignore title
    beatmap >> temp;  // ignore level

    beatmap >> num_of_notes;

    for(int i = 0; i < num_of_notes ; ++i){
        Note* note = new Normal_note(750, 150 , 130 , 0.5 , Normal_note::normal_note_type::red_note , this);
        notes.append(note);
        int start_time;
        beatmap >> start_time;
        qDebug() << temp.c_str(); // start time
        QTimer::singleShot(start_time, this, SLOT(spawn_note()));
        connect(note , SIGNAL(note_was_missed()) , this , SLOT(handle_note_miss_signal()));
        connect(note , SIGNAL(note_was_hit(int)) , this , SLOT(handle_note_hit_signal(int)));
        beatmap >> temp;
        qDebug() << temp.c_str(); // note type
        beatmap >> temp;
        qDebug() << temp.c_str(); // speed
    }
    //    notes[0]->init(scene);
    //    notes[0]->start_move();
    //    for(int i = 0 ; i < 10 ; ++i){
    //        int value = QRandomGenerator::global()->generate()%2;
    //        //        Note* note = new Normal_note(650 + i * 100, 150 , 130 , 0.5 , Normal_note::note_type::blue_note , this);
    //        Note* note;
    //        if(value == 0){
    //            note = new Normal_note(650 + i * 50, 150 , 130 , 0.5 , Normal_note::note_type::red_note , this);
    //        }else{
    //            note = new Normal_note(650 + i * 50, 150 , 130 , 0.5 , Normal_note::note_type::blue_note , this);
    //        }
    //        note->init(scene);
    //        note->start_move();
    //        note_queue.enqueue(note);
    //        connect(note, SIGNAL(die()) , this , SLOT(dequeue()));
    //    }
}


void Note_controller::judge_note(hit_type hit_type)
{
    // if there is note showing on the screen
    if(!showing_notes.isEmpty()){

        double center = 160;
        double perfect_range = 20;
        double good_range = 40;
        double bad_range = 60;
        // if it is within the perfect range, send hit signal to note
        if(center - perfect_range < showing_notes.head()->getX() && showing_notes.head()->getX() < center + perfect_range){
            Note* note = showing_notes.dequeue();
            note->get_hit(perfect);
        }
        // if it is within the good range, send hit signal to note
        else if(center - good_range < showing_notes.head()->getX() && showing_notes.head()->getX() < center + good_range){
            Note* note = showing_notes.dequeue();
            note->get_hit(good);
        }
        //        else if(center - bad_range< showing_notes.head()->getX() && showing_notes.head()->getX() < center + bad_range){
        //            Note* note = showing_notes.dequeue();
        //            note->get_hit(bad);
        //        }
    }
}

void Note_controller::dequeue()
{
    if(!showing_notes.isEmpty()){
        showing_notes.dequeue();
    }
}

void Note_controller::spawn_note()
{
    Note* note = notes.dequeue();
    note->init(scene);
    note->start_move();
    showing_notes.enqueue(note);
    connect(note, SIGNAL(die()) , this , SLOT(dequeue()));
}

void Note_controller::handle_note_hit_signal(int performance)
{
    if(performance == 0){
        emit update_performance(taiko_performance_view::Update_type::Perfect);
    }else if(performance == 1){
        emit update_performance(taiko_performance_view::Update_type::Good);
    }
}

void Note_controller::handle_note_miss_signal()
{
    emit update_performance(taiko_performance_view::Update_type::Miss);
}


