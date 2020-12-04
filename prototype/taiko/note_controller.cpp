#include "normal_note.h"
#include "note_controller.h"

#include <QRandomGenerator>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <fstream>
#include <string>
#include <QTimer>
#include <QList>
#include <QSettings>

Note_controller::Note_controller(QString beatmap_path, bool random_mode , bool fade_out_mode , bool auto_mode , QObject *parent) : beatmap_path(beatmap_path) , random_mode(random_mode) , fade_out_mode(fade_out_mode) , auto_mode(auto_mode) , QObject(parent)
{
    connect(&timer , SIGNAL(timeout()) , this , SLOT(check_is_time_spawn_note()));
}

void Note_controller::setScene(QGraphicsScene* scene)
{
    this->scene = scene;
}

void Note_controller::start()
{
    if(random_mode){
        // create new seed base on current time
        srand (time(NULL));
    }

    // prepare a Qtime to count the time elapsed
    count_time = new QTime();

    // reset the last elasped time
    last_elasped_time = 0;

    // index of the next note to be spawn
    current_index = 0;
    // check note to be spawn for every 10 ms
    timer.start(10);
    count_time->start();

    std::ifstream beatmap(beatmap_path.toStdString());

    std::string temp;
    beatmap >> temp;  // ignore map name
    beatmap >> temp;  // ignore duration
    beatmap >> temp;  // ignore creator
    beatmap >> temp;  // ignore difficulty
    beatmap >> temp;  // ignore song name

    beatmap >> offset;

    beatmap >> num_of_notes;

    // retrieve settings
    QSettings setting("HKUST" , "ORZ");

    notes_start_time = new int[num_of_notes];
    for(int i = 0; i < num_of_notes ; ++i){
        int start_time;
        beatmap >> start_time;

        int note_type;
        beatmap >> note_type;

        int speed;
        beatmap >> speed;

        Note* note;
        if(random_mode){
            note_type = rand() % 2;
        }
        note = new Normal_note(800, 150 , 130 , 0.5 , Normal_note::normal_note_type(note_type) , this);
        if(fade_out_mode){
            note->set_fade_out(fade_out_mode);
        }
        if(auto_mode){
            note->set_auto(auto_mode);
        }
        notes.append(note);
        notes_start_time[i] = start_time + offset + setting.value("offset").toInt();
        connect(note , SIGNAL(note_was_missed()) , this , SLOT(handle_note_miss_signal()));
        connect(note , SIGNAL(note_was_hit(int)) , this , SLOT(handle_note_hit_signal(int)));
    }

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
            if(((Normal_note*)showing_notes.head())->type == hit_type){
                Note* note = showing_notes.dequeue();
                note->get_hit(perfect);
            }
        }
        // if it is within the good range, send hit signal to note
        else if(center - good_range < showing_notes.head()->getX() && showing_notes.head()->getX() < center + good_range){
            if(((Normal_note*)showing_notes.head())->type == hit_type){
                Note* note = showing_notes.dequeue();
                note->get_hit(good);
            }
        }
        //        else if(center - bad_range< showing_notes.head()->getX() && showing_notes.head()->getX() < center + bad_range){
        //            Note* note = showing_notes.dequeue();
        //            note->get_hit(bad);
        //        }
    }
}

void Note_controller::pause()
{
    // the game is not started yet
    if(count_time == nullptr){
        return;
    }

    // Qtime do not provide stop function so I have to delete it and make a new one
    last_elasped_time += count_time->elapsed();
    delete count_time;

    QList<Note*> temp;

    // get all showing notes and stop them
    while(!showing_notes.isEmpty()){
        temp.append(showing_notes.dequeue());
    }
    for(int i = 0 ; i < temp.count() ; ++i){
        temp.at(i)->stop();
        showing_notes.enqueue(temp.at(i));
    }
}

void Note_controller::resume()
{
    // get all showing notes and let them move again
    QList<Note*> temp;
    while(!showing_notes.isEmpty()){
        temp.append(showing_notes.dequeue());
    }
    for(int i = 0 ; i < temp.count() ; ++i){
        temp.at(i)->unstop();
        showing_notes.enqueue(temp.at(i));
    }

    count_time = new QTime();
    count_time->start();

}

void Note_controller::restart()
{
    while(!showing_notes.isEmpty()){
        delete showing_notes.dequeue();
    }
    while(!notes.isEmpty()){
        delete notes.dequeue();
    }
    start();
}

bool Note_controller::is_auto() const
{
    return auto_mode;
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

void Note_controller::check_is_time_spawn_note()
{
    // when it is time to spawn a note, spawn it
    // need /10*10 because sometime it last digit is not zero even it is set to be start(10), so use the property of int to make it be 0 again
    if(count_time != nullptr){
        int time = (count_time->elapsed() + last_elasped_time)/10*10;
        if(time == notes_start_time[current_index]/10*10){
            spawn_note();
            ++current_index;
        }
    }
}


