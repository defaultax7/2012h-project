#ifndef NORMAL_NOTE_H
#define NORMAL_NOTE_H

#include "note.h"

class Normal_note : public Note
{

public:
    enum note_type { red_note , blue_note };
    Normal_note(double x, double y, double endpoint, double speed, note_type type, QObject* parent);
    void get_hit();
    void init(QGraphicsScene&);
private:
    note_type type;
};

#endif // NORMAL_NOTE_H
