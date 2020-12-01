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

    note_controller.init("");

    note_controller.setScene(&scene);

    connect(timer , SIGNAL(timeout()) , &note_controller , SLOT(start()));

    drum_sound_player.setSource(QUrl::fromLocalFile(":/sound_effect/sound_effect/drum_sound.wav"));
    rim_sound_player.setSource(QUrl::fromLocalFile(":/sound_effect/sound_effect/rim_sound.wav"));
}

taiko_window::~taiko_window()
{
    delete ui;
}

void taiko_window::keyPressEvent(QKeyEvent *event)
{
    // should be a set of key, but use j and f for now
    // bad design :(
    if(event->key() == Qt::Key_J){

        judge_note(hit_type::drum);
        drum_sound_player.play();
        play_drum_flash(":/image/image/drum_r.png" , 70 , 145);

        p_view.update(taiko_performance_view::Update_type::Good);

    }else if(event->key() == Qt::Key_F){

        judge_note(hit_type::drum);
        drum_sound_player.play();
        play_drum_flash(":/image/image/drum_l.png" , 30 , 145);

        p_view.update(taiko_performance_view::Update_type::Bad);

    }else if(event->key() == Qt::Key_D){

        judge_note(hit_type::rim);
        rim_sound_player.play();
        play_drum_flash(":/image/image/rim_l.png" , 17 , 131);

    }else if(event->key() == Qt::Key_K){

        judge_note(hit_type::rim);
        rim_sound_player.play();
        play_drum_flash(":/image/image/rim_r.png" , 70 , 131);

    }
    else if(event->key() == Qt::Key_1){
        note_controller.start();
    }
}

void taiko_window::testing(){
    qDebug() << "testing";
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

    int y = 135;
    int x = 190;
    int height = 100;  // does not matter how high it is
    int width_base = 1;
    perfect_judge = scene.addRect(x,y,width_base,height , QPen(QColor(0 , 255 , 0)));
    good_judge = scene.addRect(x - width_base*25/2,y,width_base*50,height , QPen(QColor(0 , 0 , 255)));
    //    bad_judge = scene.addRect(x - width_base*5/2,y,width_base*10,height , QPen(QColor(255 , 0 , 0)));
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

void taiko_window::judge_note(taiko_window::hit_type type)
{
    QList<QGraphicsItem *> temp;
    temp = perfect_judge->collidingItems();
    if(temp.count() > 3){
        if(temp.at(0)->sceneBoundingRect().width() < 60){
            note_controller.judge(type,0);
            return;
        }
    }

    QList<QGraphicsItem *> temp2;
    temp2 = good_judge->collidingItems();
    if(temp2.count() > 3){
        if(temp2.at(0)->sceneBoundingRect().width() < 60){
            note_controller.judge(type,1);
        }
    }
}


