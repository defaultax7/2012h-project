#include "music_player_window.h"
#include "ui_music_player_window.h"
#include "QDebug"

#include <QFileDialog>

music_player_window::music_player_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::music_player_window)
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

music_player_window::~music_player_window()
{
    delete ui;
}

void music_player_window::closeEvent(QCloseEvent *){
    parentWidget()->show();
}

void music_player_window::on_search_song_textChanged(const QString &arg1)
{
    qDebug() << ui->search_song->text();
}

void music_player_window::on_btn_mute_clicked()
{
    is_mute = !is_mute;
    if(is_mute){
        ui->btn_mute->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
    }else{
        ui->btn_mute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    }
}

void music_player_window::on_btn_open_music_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Map File"), ".", tr("Text Files (*.txt)"));

    //    QFileDialog fileDialog(this);
    //    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    //    fileDialog.setWindowTitle(tr("Open Files"));
    //    QStringList supportedMimeTypes = m_player->supportedMimeTypes();
    //    if (!supportedMimeTypes.isEmpty()) {
    //        supportedMimeTypes.append("audio/x-m3u");
    //        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    //    }
    //    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    //    if (fileDialog.exec() == QDialog::Accepted)
    //        addToPlaylist(fileDialog.selectedUrls());
}
