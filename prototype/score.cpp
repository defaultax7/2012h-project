#include "score.h"

Score::Score()
{

}

void Score::reset_score()
{

}

void Score::add_score(int delta)
{
    point += delta;
    refresh_score();
}

void Score::refresh_score()
{
    score_lb->setText(QString::number(point));
}

void Score::set_label(QLabel * lb)
{
    score_lb = lb;
}
