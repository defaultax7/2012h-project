#include "music_player_window.h"
#include "ui_music_player_window.h"
#include "QDebug"

#include <QFileDialog>
#include <QMessageBox>

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
    ui->btn_delete_all->setIcon(style()->standardIcon(QStyle::SP_MessageBoxCritical));

    // more space for path
    ui->treeWidget->setColumnWidth(0,250);

    // connect my player to tree list
    connect(&player , SIGNAL(song_list_changed(QLinkedList<QString>)) , this , SLOT(update_song_list(QLinkedList<QString>)));
    connect(&player , SIGNAL(current_time_update(qint64)) , this , SLOT(update_current_time(qint64)));
    connect(&player , SIGNAL(duration_update(qint64)) , this , SLOT(update_duration(qint64)));
    connect(&player , SIGNAL(song_update(QString)) , this , SLOT(update_current_song(QString)));
    connect(&player , SIGNAL(update_start_button(QMediaPlayer::State)) , this , SLOT(start_button_update(QMediaPlayer::State)));
    connect(&player , SIGNAL(auto_next_song()) , this , SLOT(auto_next_song()));

    this->setFixedSize(this->size());
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
    // clear the song list view
    ui->treeWidget->clear();

    QLinkedList<QString> song_list = player.get_filtered_song_list(arg1);

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
    QModelIndexList list = ui->treeWidget->selectionModel()->selectedIndexes();
    if(!list.isEmpty()){
        player.remove_song(list.at(0).data().toString());
    }
}

void music_player_window::on_btn_open_folder_clicked()
{
    // ask a folder
    //    QString dir_path = QFileDialog::getExistingDirectory(this, tr("Select a folder"), nullptr, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    // use for testing
    QString dir_path = "F:/music";

    // get mp3/wav inside the folder
    QDir dir(dir_path);
    QFileInfoList songs = dir.entryInfoList(QStringList() << "*.mp3" << "*.wav" ,QDir::Files);
    if(songs.isEmpty()){
        QMessageBox msgBox;
        msgBox.setText("No song found in the folder!");
        msgBox.exec();
        return;
    }
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

void music_player_window::on_btn_delete_all_clicked()
{
    QMessageBox mesBox;
    mesBox.setWindowTitle("Ask for confirmation.");
    mesBox.setText("Do you really want to remove all songs?");
    mesBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    mesBox.setDefaultButton(QMessageBox::No);
    int answer = mesBox.exec();
    if(answer == QMessageBox::Yes){
        player.remove_all();
    }
}

void music_player_window::on_btn_start_clicked()
{
    if(player.get_state() == QMediaPlayer::State::PausedState || player.get_state() == QMediaPlayer::State::StoppedState){
        player.play();
    }else if(player.get_state() == QMediaPlayer::State::PlayingState){
        player.pause();
    }
}

void music_player_window::on_btn_stop_clicked()
{
    player.stop();
}

void music_player_window::on_horizontalSlider_valueChanged(int value)
{
    player.change_volume(value);
}

void music_player_window::on_btn_next_clicked()
{
    player.next();
}

void music_player_window::on_btn_previous_clicked()
{
    player.prev();
}

void music_player_window::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QModelIndexList list = ui->treeWidget->selectionModel()->selectedIndexes();
    if(!list.isEmpty()){
        player.play_song(list.at(0).data().toString());
    }
}

QString music_player_window::number_to_timestring(qint64 current_time)
{
    QString hour = QString::number(current_time / 1000 / 3600);
    if(hour.toInt() < 10){
        hour = "0" + hour;
    }
    QString min = QString::number(current_time / 1000 / 60 % 60);
    if(min.toInt() < 10){
        min = "0" + min;
    }
    QString sec = QString::number(current_time / 1000 % 60);
    if(sec.toInt() < 10){
        sec = "0" + sec;
    }
    return hour + ":"  + min + ":" + sec;
}

void music_player_window::update_duration(qint64 new_duration)
{
    // sometime the duration is not correct is because the Microsoft DirectShow that Qmediaplayer is using
    // https://bugreports.qt.io/browse/QTBUG-34306
    ui->lb_duration->setText(number_to_timestring(new_duration));
    ui->prograss_bar->setMaximum(new_duration);
}

void music_player_window::update_current_time(qint64 current_time)
{
    ui->lb_current_time->setText(number_to_timestring(current_time));
    // if user is holding on the progress bar, dont update the current time
    if(!(ui->prograss_bar->isSliderDown())){
        ui->prograss_bar->setValue(current_time);
    }
}

void music_player_window::update_current_song(QString current_song)
{
    // trim the whole path to file name only
    QFileInfo fileInfo(current_song);
    QString song_name(fileInfo.fileName());
    // update the current song view
    ui->txt_current_song->setText(song_name);
}

void music_player_window::on_prograss_bar_sliderReleased()
{
    // jump to the time that the slider placing on
    player.jump_to(ui->prograss_bar->value());
}

void music_player_window::start_button_update(QMediaPlayer::State state)
{
    // handle the state change signal from my player, change the start button base on the state of the player
    if(state == QMediaPlayer::State::StoppedState || state == QMediaPlayer::State::PausedState){
        ui->btn_start->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }else if(state == QMediaPlayer::State::PlayingState){
        ui->btn_start->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
}

void music_player_window::auto_next_song()
{
    // handle the next song signal from my player, if auto next checkbox is ticked, play next song
    if(ui->cb_auto_next->checkState() == Qt::CheckState::Checked){
        player.next();
    }
}

void music_player_window::on_prograss_bar_valueChanged(int value)
{
    ui->lb_current_time->setText(number_to_timestring(value));
}
