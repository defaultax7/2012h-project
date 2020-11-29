#ifndef NOTE_GENERATOR_H
#define NOTE_GENERATOR_H

#include <QObject>


class note_generator : public QObject
{
    Q_OBJECT
public:
    note_generator(QObject *parent = nullptr);
};

#endif // NOTE_GENERATOR_H
