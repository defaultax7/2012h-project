#include "taiko_performance_view.h"

void taiko_performance_view::set_perfect_label(QLabel * lb)
{
    perfect_lb = lb;
}

void taiko_performance_view::set_good_label(QLabel * lb)
{
    good_lb = lb;
}

void taiko_performance_view::set_bad_label(QLabel * lb)
{
    bad_lb = lb;
}

void taiko_performance_view::set_miss_label(QLabel * lb)
{
    miss_lb = lb;
}

void taiko_performance_view::set_note_left_label(QLabel * lb)
{
    note_left_lb = lb;
}

void taiko_performance_view::set_combo_label(QLabel * lb)
{
    combo_lb = lb;
}

void taiko_performance_view::set_score(Score & score)
{
    this->score = score;
}

void taiko_performance_view::refresh_UI() const
{
    note_left_lb->setText(QString::number(note_left));
    perfect_lb->setText(QString::number(perfect));
    good_lb->setText(QString::number(good));
    miss_lb->setText(QString::number(miss));
    bad_lb->setText(QString::number(bad));
    combo_lb->setText(QString::number(combo));
    score.refresh_score();
}

void taiko_performance_view::set_note_left(int note_left)
{
    this->note_left = note_left;
}

void taiko_performance_view::update(taiko_performance_view::Update_type type)
{
    if(type == Update_type::Perfect || type == Update_type::Good){
        if(type == Update_type::Perfect){
            ++perfect;
        }else if(type == Update_type::Good){
            ++good;
        }
        ++combo;
        score.add_score(100);
        if(combo > highest_combo){
            highest_combo = combo;
        }
    }else if(type == Update_type::Bad){
        ++bad;
        combo = 0;
    }else if(type == Update_type::Miss){
        ++miss;
        combo = 0;
    }
    --note_left;
    refresh_UI();
}