#ifndef NORMAL_NOTE_H
#define NORMAL_NOTE_H

#include "note.h"

class Normal_note : public Note
{
    Q_OBJECT
public:
    enum note_type { red_note , blue_note };
    Normal_note(double x, double y, double endpoint, double speed, note_type type, QObject* parent);
    void get_hit(int performance);
    void init(QGraphicsScene*);
    Normal_note::note_type get_note_type() const;
private:
    note_type type;
private slots:
    void self_destruct();
};

#endif // NORMAL_NOTE_H
