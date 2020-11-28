#include "note.h"
#include "taiko_window.h"
#include "ui_taiko_window.h"

#include <QSound>
#include <QDebug>
#include <QPixmap>
#include <QGraphicsPixmapItem>

taiko_window::taiko_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::taiko_window)
{
    ui->setupUi(this);

    p_view.set_note_left(100);


    p_view.set_perfect_label(ui->lb_perfect_count);
    p_view.set_good_label(ui->lb_good_count);
    p_view.set_bad_label(ui->lb_bad_count);
    p_view.set_miss_label(ui->lb_miss_count);
    p_view.set_combo_label(ui->lb_roll_count);
    p_view.set_note_left_label(ui->lb_note_left);
    Score score;
    score.set_label(ui->lb_score);
    p_view.set_score(score);

    timer = new QTimer();
}

taiko_window::~taiko_window()
{
    delete ui;
}

void taiko_window::keyPressEvent(QKeyEvent *event)
{
    // should be a set of key, but use j and f for now
    if(event->key() == Qt::Key_J){
        QSound::play(":/sound_effect/sound_effect/drum_sound.wav");  // play intro sound effect
        p_view.update(taiko_performance_view::Update_type::Good);
    }else if(event->key() == Qt::Key_F){
        QSound::play(":/sound_effect/sound_effect/rim_sound.wav");  // play intro sound effect
        p_view.update(taiko_performance_view::Update_type::Bad);
    }else if(event->key() == Qt::Key_1){

        QGraphicsPixmapItem *pix_item;
        pix_item = scene.addPixmap(QPixmap(":/image/image/red_note.png"));
        Note* note = new Note(this);
        note->init(pix_item , 600.0 , 150.0);

        note->start_move();

    }
}

void taiko_window::testing(){
//    qDebug() << "testing";
}

void taiko_window::showEvent(QShowEvent *event)
{
    QImage bg;
    bg.load(":/image/image/taiko_bg.png");
//    scene.setBackgroundBrush(bg);
    scene.addPixmap(QPixmap(":/image/image/taiko_bg.png"));
    ui->graphicsView->setScene(&scene);
    ui->graphicsView->fitInView(scene.sceneRect(),Qt::KeepAspectRatio);

}
