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

void my_player::remove_song(QString)
{

}

void my_player::shuffle_song_list()
{

}






