#ifndef TAIKO_PERFORMANCE_VIEW_H
#define TAIKO_PERFORMANCE_VIEW_H

#include <QLabel>



class taiko_performance_view
{
public:
    enum Update_type{Perfect , Good , Bad , Miss , Roll};
    taiko_performance_view() = default;
    void set_perfect_label(QLabel*);
    void set_good_label(QLabel*);
    void set_bad_label(QLabel*);
    void set_miss_label(QLabel*);
    void set_note_left_label(QLabel*);
    void set_roll_label(QLabel*);
    void refresh_UI() const;
    void set_note_left(int);
    void update(Update_type);

private:
    unsigned short note_left = 0;
    unsigned short perfect = 0;
    unsigned short miss = 0;
    unsigned short good = 0;
    unsigned short roll = 0;
    unsigned short bad = 0;
    QLabel* note_left_lb;
    QLabel* perfect_lb;
    QLabel* good_lb;
    QLabel* bad_lb;
    QLabel* miss_lb;
    QLabel* roll_lb;
};

#endif // TAIKO_PERFORMANCE_VIEW_H
