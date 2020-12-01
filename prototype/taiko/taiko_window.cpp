#include "normal_note.h"
#include "note.h"
#include "taiko_window.h"
#include "ui_taiko_window.h"

#include <QSound>
#include <QDebug>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QTime>

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

    this->setFixedSize(this->size());  // prevent resizing

    note_controller.setScene(&scene);
    note_controller.init("");

    drum_sound_player.setSource(QUrl::fromLocalFile(":/sound_effect/sound_effect/drum_sound.wav"));
    rim_sound_player.setSource(QUrl::fromLocalFile(":/sound_effect/sound_effect/rim_sound.wav"));

    // set the music source for music player & bind it with progress bar
    music_player.setMedia(QUrl("F:/testing/1.mp3"));
    connect(&music_player, SIGNAL(durationChanged(qint64)), this, SLOT(duration_change(qint64)));
    connect(&music_player, SIGNAL(positionChanged(qint64)), this, SLOT(current_time_change(qint64)));

    // emit note hit/miss signal to update performance view
    connect(&note_controller, SIGNAL(update_performance(taiko_performance_view::Update_type)), &p_view, SLOT(update_performance(taiko_performance_view::Update_type)));

    // start the game after 4 second
    QTimer::singleShot(4000, this, SLOT(start_game()));
}

taiko_window::~taiko_window()
{
    delete ui;
}

void taiko_window::keyPressEvent(QKeyEvent *event)
{
    // should be a set of key, but use j and f for now
    if(event->key() == Qt::Key_J){

        note_controller.judge_note(Note_controller::hit_type::drum);
        drum_sound_player.play();
        play_drum_flash(":/image/image/drum_r.png" , 70 , 145);

//        p_view.update(taiko_performance_view::Update_type::Good);

    }else if(event->key() == Qt::Key_F){

        note_controller.judge_note(Note_controller::hit_type::drum);
        drum_sound_player.play();
        play_drum_flash(":/image/image/drum_l.png" , 30 , 145);

//        p_view.update(taiko_performance_view::Update_type::Bad);

    }else if(event->key() == Qt::Key_D){

        note_controller.judge_note(Note_controller::hit_type::rim);
        rim_sound_player.play();
        play_drum_flash(":/image/image/rim_l.png" , 17 , 131);

    }else if(event->key() == Qt::Key_K){

        note_controller.judge_note(Note_controller::hit_type::rim);
        rim_sound_player.play();
        play_drum_flash(":/image/image/rim_r.png" , 70 , 131);

    }else if(event->key() == Qt::Key_1){
        // spawn note to test
        QTimer::singleShot(1, this, SLOT(start_game()));
    }

}

void taiko_window::testing(){
    qDebug() << "testing";
}

void taiko_window::start_game()
{
    music_player.play();
    note_controller.start();
}

void taiko_window::showEvent(QShowEvent *event)
{
    // load the background image
    QImage bg;
    bg.load(":/image/image/taiko_bg.png");
    scene.addPixmap(QPixmap(":/image/image/taiko_bg.png"));
    ui->graphicsView->setScene(&scene);

    // let the image fit the whole graphic view
    ui->graphicsView->fitInView(scene.sceneRect(),Qt::KeepAspectRatio);

    judge = scene.addPixmap(QPixmap(":/image/image/judging_ring.png"));
    judge->setPos(130,120);

}

void taiko_window::play_drum_flash(QString image_path, double x, double y)
{
    QGraphicsPixmapItem* drum_flash = scene.addPixmap(image_path);
    drum_flash->setPos(x,y);
    QTime t;
    t.start();
    while (t.elapsed() < drum_flash_time) {
        QCoreApplication::processEvents();
    }
    scene.removeItem(drum_flash);
}

void taiko_window::duration_change(qint64 new_duration)
{
    ui->music_progress_bar->setMaximum(new_duration);
}

void taiko_window::current_time_change(qint64 current_time)
{
    ui->music_progress_bar->setValue(current_time);
}




