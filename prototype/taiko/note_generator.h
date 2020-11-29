#ifndef NOTE_GENERATOR_H
#define NOTE_GENERATOR_H

#include "note.h"

#include <QGraphicsScene>
#include <QObject>
#include <QQueue>


class Note_generator : public QObject
{
    Q_OBJECT
public:
    Note_generator(QObject *parent = nullptr);
    void setScene(QGraphicsScene*);
    void init(QString beatmap_path);
    void start();
    void judge(int performance);

private slots:
    void dequeue();

private:
    QGraphicsScene* scene;
    QQueue<Note*> note_queue;
    unsigned int num_of_notes = 0;
};

#endif // NOTE_GENERATOR_H
