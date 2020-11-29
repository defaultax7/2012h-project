#ifndef NOTE_GENERATOR_H
#define NOTE_GENERATOR_H

#include <QGraphicsScene>
#include <QObject>


class Note_generator : public QObject
{
    Q_OBJECT
public:
    Note_generator(QObject *parent = nullptr);
    void setScene(QGraphicsScene*);
    void init(QString beatmap_path);
    void start();
private:
    QGraphicsScene* scene;
    unsigned int num_of_notes = 0;
};

#endif // NOTE_GENERATOR_H
