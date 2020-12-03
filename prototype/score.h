#ifndef SCORE_H
#define SCORE_H

#include <QLabel>



class Score
{
public:
    void reset_score();
    void add_score(int);
    void set_label(QLabel*);
    void refresh_score() const;
    int get_score() const;

private:
    unsigned int point = 0;
    QLabel* score_lb;
};

#endif // SCORE_H
