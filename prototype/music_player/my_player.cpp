#include "my_player.h"

#include <QFileInfo>

my_player::my_player()
{
    player = new QMediaPlayer();
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(duration_change(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(current_time_change(qint64)));
}

void my_player::setMuted(bool is_mute)
{
    player->setMuted(is_mute);
}

void my_player::add_song(QString song)
{
    if(!song_list.contains(song)){
        song_list.append(song);
    }
    emit song_list_changed(song_list);
}

void my_player::add_songs(QStringList add_list)
{
    for(int i =0; i < add_list.count(); ++i){
        add_song(add_list.at(i));
    }
}

void my_player::remove_song(QString song)
{
    for(QLinkedList<QString>::iterator it = song_list.begin() ; it != song_list.end() ; ++it){
        if(*it == song){
            song_list.erase(it);
            break;
        }
    }
    emit song_list_changed(song_list);
}

void my_player::shuffle_song_list()
{

}

void my_player::remove_all()
{
    song_list.clear();
    emit song_list_changed(song_list);
}

void my_player::play_song(QString song_path)
{
    // set current song
    for(current_song = song_list.begin() ; current_song != song_list.end() ; ++current_song ){
        if(*current_song == song_path){
            player->setMedia(QUrl(*current_song));
            player->play();
            emit song_update(song_path);
            break;
        }
    }
}

void my_player::stop()
{
    player->stop();
}

void my_player::change_volume(int volume)
{
    player->setVolume(volume);
}

void my_player::next()
{
    // it is not circular, no next for the last one
    if(current_song != song_list.end()){
        ++current_song;
    }
    if(current_song != song_list.end()){
        player->setMedia(QUrl(*current_song));
        player->play();
        emit song_update(*current_song);
    }
}

void my_player::prev()
{
    // it is not circular, no go back from first one
    if(current_song != song_list.begin()){
        --current_song;
        player->setMedia(QUrl(*current_song));
        player->play();
        emit song_update(*current_song);
    }
}

QLinkedList<QString> my_player::get_filtered_song_list(const QString& filter_string)
{
    QLinkedList<QString> filtered_list;
    // load the latest song list to the view
    for (QLinkedList<QString>::iterator it = song_list.begin(); it != song_list.end(); ++it) {
        // extract song name from abs path
        QFileInfo fileInfo(*it);
        QString song_name(fileInfo.fileName());
        QStringList temp;

        if ((*it).toStdString().find(filter_string.toStdString()) != std::string::npos) {
            filtered_list.append(*it);
        }
    }

    return filtered_list;
}

void my_player::duration_change(qint64 new_duration)
{
    emit duration_update(new_duration);
}

void my_player::current_time_change(qint64 current_time)
{
    emit current_time_update(current_time);
}







