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
    song_list.append(song);
}

void my_player::shuffle_song_list()
{

}
