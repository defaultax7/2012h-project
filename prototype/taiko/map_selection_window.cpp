#include "map_selection_window.h"
#include "ui_map_selection_window.h"

map_selection_window::map_selection_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::map_selection_window)
{
    ui->setupUi(this);

    btn_inactive_style = "QPushButton { \
            font-size: 16px;\
            font: bold large Aerial;\
            color:rgb(17,17,17);\
            border-width: 1px;\
            border-radius: 6px;\
            border-bottom-color: rgb(150,150,150);\
            border-right-color: rgb(165,165,165);\
            border-left-color: rgb(165,165,165);\
            border-top-color: rgb(180,180,180);\
            border-style: solid;\
            padding: 4px;\
            background-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(255, 255, 255, 255));\
        }";

    btn_active_style = "QPushButton{\
            font-size: 16px;\
            font: bold large Aerial;\
            color:rgb(17,17,17);\
            border-width: 2px;\
            border-radius: 6px;\
            border-top-color: rgb(76,195,255);\
            border-right-color: qlineargradient(spread:pad, x1:0, y1:1, x2:1, y2:0, stop:0 rgb(76,195,255), stop:1 rgb(76,195,255));\
            border-left-color:  qlineargradient(spread:pad, x1:1, y1:0, x2:0, y2:0, stop:0 rgb(76,195,255), stop:1 rgb(76,195,255));\
            border-bottom-color: rgb(76,195,255);\
            border-style: solid;\
            padding: 4px;\
            background-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(255, 255, 255, 255));\
        }";
}

map_selection_window::~map_selection_window()
{
    delete ui;
}

void map_selection_window::showEvent(QShowEvent *event)
{
    QPixmap pixmap(":/image/image/back_button.png");
    QIcon ButtonIcon(pixmap);
    ui->btn_back->setIcon(ButtonIcon);
    ui->btn_back->setIconSize(pixmap.rect().size()/25);
}

void map_selection_window::on_btn_auto_mode_clicked()
{
    if(!auto_mode){
        ui->btn_auto_mode->setStyleSheet(btn_active_style);
    }else{
        ui->btn_auto_mode->setStyleSheet(btn_inactive_style);
    }
    auto_mode = !auto_mode;
}

void map_selection_window::on_btn_high_speed_mode_clicked()
{
    if(!high_speed_mode){
        ui->btn_high_speed_mode->setStyleSheet(btn_active_style);
    }else{
        ui->btn_high_speed_mode->setStyleSheet(btn_inactive_style);
    }
    high_speed_mode = !high_speed_mode;
}

void map_selection_window::on_btn_dark_mode_clicked()
{
    if(!dark_mode){
        ui->btn_dark_mode->setStyleSheet(btn_active_style);
    }else{
        ui->btn_dark_mode->setStyleSheet(btn_inactive_style);
    }
    dark_mode = !dark_mode;
}

void map_selection_window::on_btn_fade_out_mode_clicked()
{
    if(!fade_out_mode){
        ui->btn_fade_out_mode->setStyleSheet(btn_active_style);
    }else{
        ui->btn_fade_out_mode->setStyleSheet(btn_inactive_style);
    }
    fade_out_mode = !fade_out_mode;
}

void map_selection_window::on_btn_random_mode_clicked()
{
    if(!random_mode){
        ui->btn_random_mode->setStyleSheet(btn_active_style);
    }else{
        ui->btn_random_mode->setStyleSheet(btn_inactive_style);
    }
    random_mode = !random_mode;
}
