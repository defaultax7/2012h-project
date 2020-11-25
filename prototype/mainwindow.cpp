#include "mainwindow.h"
#include "music_player_window.h"
#include "ui_mainwindow.h"

#include <QSound>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height()); // prevent resize
//    QSound::play(":/sound_effect/sound_effect/welcome.wav");  // play intro sound effect
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_exit_btn_clicked()
{
    MainWindow::close();
}

void MainWindow::on_music_btn_clicked()
{
    music_player_window* mp = new music_player_window(this);
    mp->show();
    hide();
}
