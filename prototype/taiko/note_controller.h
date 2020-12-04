#ifndef NOTE_CONTROLLER_H
#define NOTE_CONTROLLER_H

#include "note.h"
#include "taiko_performance_view.h"

#include <QGraphicsScene>
#include <QObject>
#include <QQueue>
#include <QTime>


class Note_controller : public QObject
{
    Q_OBJECT
public:
    enum hit_type{drum , rim};
    enum performance{perfect, good, bad};
    Note_controller(QString beatmap_path, bool random_mode , bool fade_out_mode , bool auto_mode , QObject *parent = nullptr);
    void setScene(QGraphicsScene*);
    void start();
    void judge_note(hit_type hit_type);
    void pause();
    void resume();
    void restart();
    bool is_auto() const;

signals:
    void update_performance(taiko_performance_view::Update_type);

private slots:
    void dequeue();
    void spawn_note();
    void handle_note_hit_signal(int);
    void handle_note_miss_signal();
    void check_is_time_spawn_note();

private:
    QGraphicsScene* scene;
    QQueue<Note*> showing_notes;
    QQueue<Note*> notes;
    QTimer timer;
    QTime* count_time = nullptr;
    QString beatmap_path;
    unsigned int num_of_notes = 0;
    int* notes_start_time;
    int current_index = 0;
    int last_elasped_time = 0;
    int offset = 0;
    bool random_mode = false;
    bool fade_out_mode = false;
    bool auto_mode  = false;
};

#endif // NOTE_GENERATOR_H
