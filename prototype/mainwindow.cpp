#include "mainwindow.h"
#include "option_menu_window.h"
#include "ui_mainwindow.h"

#include <QSound>

#include <music_player/music_player_window.h>

#include <taiko/taiko_window.h>

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

void MainWindow::showEvent(QShowEvent * event){
    QPixmap pixmap(":/image/image/taiko_start_button.png");
    QIcon ButtonIcon(pixmap);
    ui->btn_start_taiko->setIcon(ButtonIcon);
    ui->btn_start_taiko->setIconSize(pixmap.rect().size()/2);
}

void MainWindow::on_exit_btn_clicked()
{
    MainWindow::close();
}

void MainWindow::on_music_btn_clicked()
{
    music_player_window* w = new music_player_window(this);
    w->show();
    hide();
}

void MainWindow::on_option_btn_clicked()
{
    option_menu_window* w = new option_menu_window(this);
    w->show();
    hide();
}

void MainWindow::on_btn_start_taiko_clicked()
{
    taiko_window* w = new taiko_window(this);
    w->show();
    hide();
}
