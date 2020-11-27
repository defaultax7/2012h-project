#include "score.h"


void Score::reset_score()
{
    point = 0;
    refresh_score();
}

void Score::add_score(int delta)
{
    point += delta;
    refresh_score();
}

void Score::refresh_score() const
{
    score_lb->setText(QString::number(point));
}

void Score::set_label(QLabel * lb)
{
    score_lb = lb;
}
