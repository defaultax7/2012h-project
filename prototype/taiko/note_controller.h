#ifndef NOTE_CONTROLLER_H
#define NOTE_CONTROLLER_H

#include "note.h"
#include "taiko_performance_view.h"

#include <QGraphicsScene>
#include <QObject>
#include <QQueue>


class Note_controller : public QObject
{
    Q_OBJECT
public:
    enum hit_type{drum , rim};
    enum performance{perfect, good, bad};
    Note_controller(QObject *parent = nullptr);
    void setScene(QGraphicsScene*);
    void init(QString beatmap_path);
    void start();
    void judge_note(hit_type hit_type);

signals:
    void update_performance(taiko_performance_view::Update_type);

private slots:
    void dequeue();
    void spawn_note();
    void handle_note_hit_signal(int);
    void handle_note_miss_signal();

private:
    QGraphicsScene* scene;
    QQueue<Note*> showing_notes;
    unsigned int num_of_notes = 0;
    QQueue<Note*> notes;
};

#endif // NOTE_GENERATOR_H
