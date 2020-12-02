#ifndef TAIKO_PERFORMANCE_VIEW_H
#define TAIKO_PERFORMANCE_VIEW_H

#include "score.h"

#include <QLabel>



class taiko_performance_view : public QObject
{
    Q_OBJECT
public:
    enum Update_type{Perfect , Good , Bad , Miss};
    taiko_performance_view(QObject* parent= nullptr);
    void set_perfect_label(QLabel*);
    void set_good_label(QLabel*);
    void set_bad_label(QLabel*);
    void set_miss_label(QLabel*);
    void set_note_left_label(QLabel*);
    void set_combo_label(QLabel*);
    void set_score(Score&);
    void refresh_UI() const;
    void set_note_left(int);
    void reset();

private slots:
    void update_performance(taiko_performance_view::Update_type);

private:
    unsigned int original_note_left = 0;
    unsigned int note_left = 0;
    unsigned int perfect = 0;
    unsigned int miss = 0;
    unsigned int good = 0;
    unsigned int combo = 0;
    unsigned int bad = 0;
    unsigned int highest_combo = 0;
    unsigned int base_score_per_note = 100;
    unsigned int max_score_per_note = 200;
    unsigned int combo_extra_score = 10; // extra score for note for every 10 combo
    QLabel* note_left_lb;
    QLabel* perfect_lb;
    QLabel* good_lb;
    QLabel* bad_lb;
    QLabel* miss_lb;
    QLabel* combo_lb;
    Score score;
};

#endif // TAIKO_PERFORMANCE_VIEW_H
