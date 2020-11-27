#ifndef TAIKO_PERFORMANCE_VIEW_H
#define TAIKO_PERFORMANCE_VIEW_H

#include "score.h"

#include <QLabel>



class taiko_performance_view
{
public:
    enum Update_type{Perfect , Good , Bad , Miss};
    taiko_performance_view() = default;
    void set_perfect_label(QLabel*);
    void set_good_label(QLabel*);
    void set_bad_label(QLabel*);
    void set_miss_label(QLabel*);
    void set_note_left_label(QLabel*);
    void set_combo_label(QLabel*);

    void set_score(Score&);

    void refresh_UI() const;
    void set_note_left(int);
    void update(Update_type);

private:
    unsigned short note_left = 0;
    unsigned short perfect = 0;
    unsigned short miss = 0;
    unsigned short good = 0;
    unsigned short combo = 0;
    unsigned short bad = 0;
    unsigned short highest_combo = 0;
    QLabel* note_left_lb;
    QLabel* perfect_lb;
    QLabel* good_lb;
    QLabel* bad_lb;
    QLabel* miss_lb;
    QLabel* combo_lb;
    Score score;
};

#endif // TAIKO_PERFORMANCE_VIEW_H
