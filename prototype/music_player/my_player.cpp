#include "my_player.h"

my_player::my_player()
{
    player = new QMediaPlayer();
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
    if(current_song != song_list.end()){
        ++current_song;
    }
    if(current_song != song_list.end()){
        player->setMedia(QUrl(*current_song));
        player->play();
    }
}

void my_player::prev()
{
    if(current_song != song_list.begin()){
        --current_song;
        player->setMedia(QUrl(*current_song));
        player->play();
    }
}






