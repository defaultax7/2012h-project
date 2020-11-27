#include "taiko_window.h"
#include "ui_taiko_window.h"

#include <QSound>
#include <QDebug>
#include <QPixmap>

taiko_window::taiko_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::taiko_window)
{
    ui->setupUi(this);

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
    }else if(event->key() == Qt::Key_F){
        QSound::play(":/sound_effect/sound_effect/rim_sound.wav");  // play intro sound effect
    }
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
