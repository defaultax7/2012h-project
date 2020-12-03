#include "mainwindow.h"
#include "option_menu_window.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <QSound>

#include <music_player/music_player_window.h>

#include <taiko/map/map_selection_window.h>
#include <taiko/taiko_window.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height()); // prevent resize
    //    QSound::play(":/sound_effect/sound_effect/welcome.wav");  // play intro sound effect

    QSettings setting("HKUST" , "ORZ");

    // init the setting for first time user
    if(!setting.value("music_vol").isValid()){
        setting.setValue("music_vol",100);
    }if(!setting.value("effect_vol").isValid()){
        setting.setValue("effect_vol",100);
    }if(!setting.value("offset").isValid()){
        setting.setValue("offset",0);
    }
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
    music_player_window* w = new music_player_window();
    w->show();
    close();
}

void MainWindow::on_option_btn_clicked()
{
    option_menu_window* w = new option_menu_window();
    w->show();
    close();
}

void MainWindow::on_btn_start_taiko_clicked()
{
    map_selection_window *w = new map_selection_window();
    w->show();
    close();
}
