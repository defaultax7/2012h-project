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
    Note_controller(QObject *parent = nullptr);
    void setScene(QGraphicsScene*);
    void init(QString beatmap_path);
    void start();
    void judge(int drum_or_rim, int performance);

private slots:
    void dequeue();

private:
    QGraphicsScene* scene;
    QQueue<Note*> note_queue;
    unsigned int num_of_notes = 0;
};

#endif // NOTE_GENERATOR_H
