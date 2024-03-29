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
#include <QGraphicsDropShadowEffect>

#include <taiko/map/map_selection_window.h>

taiko_window::taiko_window(QString map_path , QString song_path, int note_left , bool auto_mode, bool high_speed_mode, bool dark_mode, bool fade_out_mode, bool random_mode, QWidget *parent) : QMainWindow(parent), ui(new Ui::taiko_window),
    auto_mode(auto_mode) , high_speed_mode(high_speed_mode) , dark_mode(dark_mode) , fade_out_mode(fade_out_mode) , random_mode(random_mode) , note_controller(map_path, random_mode , fade_out_mode , auto_mode , high_speed_mode) , map_path(map_path)
{
    ui->setupUi(this);

    music_player = new QMediaPlayer();

    if(high_speed_mode){
        // 1.3 times faster for high speed mode
        music_player->setPlaybackRate(1.3);
    }

    // setup the performance view
    p_view.set_note_left(note_left);
    p_view.set_perfect_label(ui->lb_perfect_count);
    p_view.set_good_label(ui->lb_good_count);
    //    p_view.set_bad_label(ui->lb_bad_count);
    p_view.set_miss_label(ui->lb_miss_count);
    p_view.set_combo_label(ui->lb_roll_count);
    p_view.set_note_left_label(ui->lb_note_left);
    Score score;
    score.set_label(ui->lb_score);
    p_view.set_score(score);

    timer = new QTimer();

    this->setFixedSize(this->size());  // prevent resizing

    note_controller.setScene(&scene);

    // retrieve settings
    QSettings setting("HKUST" , "ORZ");

    // set the sound source for hitting effect
    drum_sound_player.setSource(QUrl::fromLocalFile(":/sound_effect/sound_effect/drum_sound.wav"));
    rim_sound_player.setSource(QUrl::fromLocalFile(":/sound_effect/sound_effect/rim_sound.wav"));

    // set the effect volume based on setting from option panel
    drum_sound_player.setVolume(setting.value("effect_vol").toInt());
    rim_sound_player.setVolume(setting.value("effect_vol").toInt());

    // set the music source for music player
    //    music_player.setMedia(QUrl("F:/testing/1.mp3"));
    music_player->setMedia(QUrl(song_path));

    // set the volume based on setting from option panel
    music_player->setVolume(setting.value("music_vol").toInt());
    //  bind the music player with progress bar
    connect(music_player, SIGNAL(durationChanged(qint64)), this, SLOT(duration_change(qint64)));
    connect(music_player, SIGNAL(positionChanged(qint64)), this, SLOT(current_time_change(qint64)));
    connect(music_player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(handle_music_finish_signal(QMediaPlayer::State)));

    // emit note hit/miss signal to update performance view
    connect(&note_controller, SIGNAL(update_performance(taiko_performance_view::Update_type)), &p_view, SLOT(update_performance(taiko_performance_view::Update_type)));

    // after song the is finish, there will be few second staying on this window before going to result window
    connect(timer , SIGNAL(timeout()), this , SLOT(show_result()));
    timer->setSingleShot(true);

    hide_pause_screen();

    // map the key setting to the list view
    if(setting.contains("drum_l_key")){
        drum_l_list_string = setting.value("drum_l_key").toStringList();
    }
    if(setting.contains("drum_r_key")){
        drum_r_list_string = setting.value("drum_r_key").toStringList();
    }
    if(setting.contains("rim_l_key")){
        rim_l_list_string = setting.value("rim_l_key").toStringList();
    }
    if(setting.contains("rim_r_key")){
        rim_r_list_string = setting.value("rim_r_key").toStringList();
    }

}

taiko_window::~taiko_window()
{
    delete ui;
}

void taiko_window::keyPressEvent(QKeyEvent *event)
{
    QString key_to_String = QKeySequence(event->key()).toString();

    if(drum_l_list_string.contains(key_to_String)){
        hit_drum_l();
    }else if(drum_r_list_string.contains(key_to_String)){
        hit_drum_r();
    }else if(rim_r_list_string.contains(key_to_String)){
        hit_rim_r();
    }else if(rim_l_list_string.contains(key_to_String)){
        hit_rim_l();
    }else if(event->key() == Qt::Key_Space){
        start_game();
    }else if(event->key() == Qt::Key_Escape){
        pause();
    }

}


void taiko_window::start_game()
{
    // start to play the song and spawn note
    music_player->play();
    note_controller.start();
    // hide the promt start label
    ui->lb_promt_start->setVisible(false);
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

    // put the judging area image to the scene
    judge = scene.addPixmap(QPixmap(":/image/image/judging_ring.png"));
    judge->setPos(130,120);

    if(dark_mode){
        // add a black rect to block the vision
        QGraphicsRectItem* black = scene.addRect(  400 , 117 , 470 , 130 , QPen() , QBrush(Qt::black));
        // let it always on top
        black->setZValue(1);
    }

    // apply shadow effect for performance frame to make it look better
    QGraphicsDropShadowEffect* show_effect = new QGraphicsDropShadowEffect();
    show_effect->setBlurRadius(20);
    ui->graphicsView_2->setGraphicsEffect(show_effect);

    p_view.refresh_UI();

}

void taiko_window::closeEvent(QCloseEvent *)
{
    delete music_player;
    delete timer;
    // if it is going to the result window, map_selection_window should not be shown
    if(!showing_result){
        map_selection_window* w = new map_selection_window();
        w->show();
    }
    close();
}

void taiko_window::play_drum_flash(QString image_path, double x, double y)
{
    // show the drum flash
    QGraphicsPixmapItem* drum_flash = scene.addPixmap(image_path);
    drum_flash->setPos(x,y);
    QTime t;
    t.start();
    // proceed to other process
    while (t.elapsed() < drum_flash_time) {
        QCoreApplication::processEvents();
    }
    // remove the flash when time is reached
    scene.removeItem(drum_flash);
}

void taiko_window::show_result()
{
    result_window* w = new result_window(p_view.get_performance() , map_path , auto_mode);
    w->show();
    close();
}

void taiko_window::pause()
{
    show_pause_screen();
    music_player->pause();
    note_controller.pause();
}

void taiko_window::resume()
{
    hide_pause_screen();
    music_player->play();
    note_controller.resume();
}

void taiko_window::retry()
{
    // restart the music
    music_player->setPosition(0);
    music_player->play();

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
    close();
}

void taiko_window::hit_drum_l()
{
    if(!auto_mode){
        note_controller.judge_note(Note_controller::hit_type::drum);
//        drum_sound_player.play();
        QSound::play(":/sound_effect/sound_effect/drum_sound.wav");
        play_drum_flash(":/image/image/drum_l.png" , 30 , 145);
    }
}

void taiko_window::hit_drum_r()
{
    if(!auto_mode){
        note_controller.judge_note(Note_controller::hit_type::drum);
//        drum_sound_player.play();
        QSound::play(":/sound_effect/sound_effect/drum_sound.wav");
        play_drum_flash(":/image/image/drum_r.png" , 70 , 145);
    }
}

void taiko_window::hit_rim_l()
{
    if(!auto_mode){
        note_controller.judge_note(Note_controller::hit_type::rim);
//        rim_sound_player.play();
        QSound::play(":/sound_effect/sound_effect/rim_sound.wav");
        play_drum_flash(":/image/image/rim_l.png" , 17 , 131);
    }
}

void taiko_window::hit_rim_r()
{
    if(!auto_mode){
        note_controller.judge_note(Note_controller::hit_type::rim);
//        rim_sound_player.play();
        QSound::play(":/sound_effect/sound_effect/rim_sound.wav");
        play_drum_flash(":/image/image/rim_r.png" , 70 , 131);
    }
}

void taiko_window::handle_music_finish_signal(QMediaPlayer::State state)
{
    if(state == QMediaPlayer::State::StoppedState && music_player->duration() > 0 && music_player->position() == music_player->duration()){
        timer->start(2000);
        showing_result = true;
    }
}
