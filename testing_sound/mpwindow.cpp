#include "mpwindow.h"
#include "ui_mpwindow.h"

mpwindow::mpwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mpwindow)
{
    ui->setupUi(this);

    // Set icon for button
    ui->btn_open_folder->setIcon(style()->standardIcon(QStyle::SP_FileDialogEnd));
    ui->btn_open_music->setIcon(style()->standardIcon(QStyle::SP_DriveCDIcon));

    ui->btn_start->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_next->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->btn_previous->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->btn_stop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->btn_mute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

}

mpwindow::~mpwindow()
{
    delete ui;
}

void mpwindow::closeEvent(QCloseEvent *){
    parentWidget()->show();
}

void mpwindow::on_btn_mute_clicked()
{

    is_mute = !is_mute;
    if(is_mute){
        ui->btn_mute->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
    }else{
        ui->btn_mute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    }
}
