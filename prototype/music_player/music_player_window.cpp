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
    ui->btn_delete->setIcon(style()->standardIcon(QStyle::SP_DialogDiscardButton));

    // more space for path
    ui->treeWidget->setColumnWidth(0,250);

    connect(&player , SIGNAL(song_list_changed(QLinkedList<QString>)) , this , SLOT(update_song_list(QLinkedList<QString>)));
    // connect my player to tree list
}


music_player_window::~music_player_window()
{
    delete ui;
}

void music_player_window::closeEvent(QCloseEvent *){
    //    parentWidget()->show();
}

void music_player_window::on_search_song_textChanged(const QString &arg1)
{
    qDebug() << ui->search_song->text();
}

void music_player_window::on_btn_mute_clicked()
{
    is_mute = !is_mute;
    if(is_mute){
        player.setMuted(true);
        ui->btn_mute->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
    }else{
        player.setMuted(false);
        ui->btn_mute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    }
}

void music_player_window::on_btn_open_music_clicked()
{
    // Ask for a music file (accept mp3 or wav only)
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select a song"),nullptr,tr("Music (*.mp3 *.wav)"));

    if(!fileName.isEmpty()){
        player.add_song(fileName);
    }

    //    if (!supportedMimeTypes.isEmpty()) {
    //        supportedMimeTypes.append("audio/x-m3u"); // MP3 playlists
    //        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    //    }
    //    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    //    if (fileDialog.exec() == QDialog::Accepted)
    //        addToPlaylist(fileDialog.selectedUrls());

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

void music_player_window::update_song_list(QLinkedList<QString> song_list)
{
    // clear the song list view
    ui->treeWidget->clear();

    // load the latest song list to the view
    for (QLinkedList<QString>::iterator it = song_list.begin(); it != song_list.end(); ++it) {
        // extract song name from abs path
        QFileInfo fileInfo(*it);
        QString song_name(fileInfo.fileName());
        QStringList temp;
        temp.append(*it);
        temp.append(song_name);
        // add the song info to tree view
        QTreeWidgetItem* song = new QTreeWidgetItem(temp);
        ui->treeWidget->addTopLevelItem(song);
    }
}

void music_player_window::on_btn_delete_clicked()
{

}

void music_player_window::on_btn_open_folder_clicked()
{
    // ask a folder
    QString dir_path = QFileDialog::getExistingDirectory(this, tr("Select a folder"), nullptr, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    // get mp3/wav inside the folder
    QDir dir(dir_path);
    QFileInfoList songs = dir.entryInfoList(QStringList() << "*.mp3" << "*.wav" ,QDir::Files);
    QStringList songs_path;
    for (int i = 0 ; i < songs.count() ; ++i) {
        songs_path.append(songs.at(i).absoluteFilePath());
    }
    player.add_songs(songs_path);

}

void music_player_window::on_btn_random_song_list_clicked()
{
    player.shuffle_song_list();
}
