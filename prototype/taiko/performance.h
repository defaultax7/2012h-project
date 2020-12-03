#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <score.h>



class Performance
{
public:
    enum rank{ S , A , B , C , D};
    Performance(int total_note , int perfect , int miss , int good , int combo , Score score);
    rank get_rank() const;
    unsigned int total_note = 0;
    unsigned int perfect = 0;
    unsigned int miss = 0;
    unsigned int good = 0;
    unsigned int highest_combo = 0;
    Score score;
};

#endif // PERFORMANCE_H
