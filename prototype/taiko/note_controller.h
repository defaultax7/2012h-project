#ifndef NOTE_CONTROLLER_H
#define NOTE_CONTROLLER_H

#include "note.h"

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

private slots:
    void dequeue();
    void spawn_note();

private:
    QGraphicsScene* scene;
    QQueue<Note*> showing_notes;
    unsigned int num_of_notes = 0;
    QQueue<Note*> notes;
};

#endif // NOTE_GENERATOR_H
