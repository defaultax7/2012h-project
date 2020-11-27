#ifndef SCORE_H
#define SCORE_H

#include <QLabel>



class Score
{
public:
    Score();
    void reset_score();
    void add_score(int);
    void refresh_score();
    void set_label(QLabel*);

private:
    int point = 0;
    QLabel* score_lb;
};

#endif // SCORE_H
