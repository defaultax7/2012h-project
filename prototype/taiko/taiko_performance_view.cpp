#include "taiko_performance_view.h"
#include "QDebug"

taiko_performance_view::taiko_performance_view(QObject *parent) : QObject(parent)
{

}

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


void taiko_performance_view::update_performance(taiko_performance_view::Update_type update_type)
{
    if(update_type == Update_type::Perfect || update_type == Update_type::Good){
        if(update_type == Update_type::Perfect){
            ++perfect;
        }else if(update_type == Update_type::Good){
            ++good;
        }
        // calculate the score to be add
        int add_score = base_score_per_note + (combo / 10) * 10;
        // select the smaller one between add_score and max_score
        score.add_score((add_score > max_score_per_note) ? max_score_per_note : add_score);
        ++combo;
        if(combo > highest_combo){
            highest_combo = combo;
        }
    }else if(update_type == Update_type::Bad){
        ++bad;
        combo = 0;
    }else if(update_type == Update_type::Miss){
        ++miss;
        combo = 0;
    }
    --note_left;
    refresh_UI();
}
