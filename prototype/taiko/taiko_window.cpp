#include "normal_note.h"
#include "note.h"
#include "result_window.h"
#include "taiko_window.h"
#include "ui_taiko_window.h"

#include <QSound>
#include <QDebug>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QTime>
#include <QSettings>

#include <taiko/map/map_selection_window.h>

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

    // retrieve settings
    QSettings setting("HKUST" , "ORZ");

    // set the sound source for hitting effect
    drum_sound_player.setSource(QUrl::fromLocalFile(":/sound_effect/sound_effect/drum_sound.wav"));
    rim_sound_player.setSource(QUrl::fromLocalFile(":/sound_effect/sound_effect/rim_sound.wav"));

    // set the effect volume based on setting from option panel
    drum_sound_player.setVolume(setting.value("effect_vol").toInt());
    rim_sound_player.setVolume(setting.value("effect_vol").toInt());

    // set the music source for music player
    music_player.setMedia(QUrl("F:/testing/1.mp3"));
    // set the volume based on setting from option panel
    music_player.setVolume(setting.value("music_vol").toInt());
    //  bind the music player with progress bar
    connect(&music_player, SIGNAL(durationChanged(qint64)), this, SLOT(duration_change(qint64)));
    connect(&music_player, SIGNAL(positionChanged(qint64)), this, SLOT(current_time_change(qint64)));
    connect(&music_player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(handle_music_finish_signal(QMediaPlayer::State)));

    // emit note hit/miss signal to update performance view
    connect(&note_controller, SIGNAL(update_performance(taiko_performance_view::Update_type)), &p_view, SLOT(update_performance(taiko_performance_view::Update_type)));

    // start the game after 4 second
    //    QTimer::singleShot(5000, this, SLOT(start_game()));

    hide_pause_screen();

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

    }else if(event->key() == Qt::Key_F){

        note_controller.judge_note(Note_controller::hit_type::drum);
        drum_sound_player.play();
        play_drum_flash(":/image/image/drum_l.png" , 30 , 145);

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
    }else if(event->key() == Qt::Key_Escape){
        pause();
    }
//    else if(event->key() == Qt::Key_0){
//        show_result();
//    }

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

void taiko_window::closeEvent(QCloseEvent *)
{
    map_selection_window* w = new map_selection_window();
    w->show();
    close();
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

void taiko_window::show_result()
{
//    result_window* w = new result_window(parent);
    result_window* w = new result_window();
    w->show();
    this->hide();
}

void taiko_window::pause()
{
    show_pause_screen();
    music_player.pause();
    note_controller.pause();
}

void taiko_window::resume()
{
    hide_pause_screen();
    music_player.play();
    note_controller.resume();
}

void taiko_window::retry()
{
    // restart the music
    music_player.setPosition(0);
    music_player.play();

    p_view.reset();

    hide_pause_screen();
    note_controller.restart();
}

void taiko_window::hide_pause_screen()
{
    ui->pause_smoke->setVisible(false);
    ui->btn_retry->setVisible(false);
    ui->btn_resume->setVisible(false);
    ui->btn_exit->setVisible(false);
}

void taiko_window::show_pause_screen()
{
    ui->pause_smoke->setVisible(true);
    ui->btn_retry->setVisible(true);
    ui->btn_resume->setVisible(true);
    ui->btn_exit->setVisible(true);
}

void taiko_window::duration_change(qint64 new_duration)
{
    ui->music_progress_bar->setMaximum(new_duration);
}

void taiko_window::current_time_change(qint64 current_time)
{
    ui->music_progress_bar->setValue(current_time);
}


void taiko_window::on_btn_resume_clicked()
{
    resume();
}

void taiko_window::on_btn_retry_clicked()
{
    retry();
}

void taiko_window::on_btn_exit_clicked()
{
    //    parentWidget()->show();
}

void taiko_window::handle_music_finish_signal(QMediaPlayer::State state)
{
    if(state == QMediaPlayer::State::StoppedState && music_player.duration() > 0 && music_player.position() == music_player.duration()){
        QTimer::singleShot(2000, this , SLOT(show_result()));
    }
}
