#ifndef PERFORMANCE_H
#define PERFORMANCE_H


class performance
{
public:
    performance();

private:
    unsigned int total_note = 0;
    unsigned int perfect = 0;
    unsigned int miss = 0;
    unsigned int good = 0;
    unsigned int combo = 0;
    unsigned int highest_combo = 0;
    unsigned int base_score_per_note = 100;
    unsigned int max_score_per_note = 200;
    unsigned int combo_extra_score = 10; // extra score for note for every 10 combo
};

#endif // PERFORMANCE_H
