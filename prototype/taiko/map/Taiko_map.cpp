#include "Taiko_map.h"

Taiko_map::Taiko_map(QString map_name, QString duration, QString creator, QString difficulty, QString root_folder, QString map_path, QString song_path, QString song_preview_path, int offset) : map_name(map_name) , duration(duration) , creator(creator) , difficulty(difficulty) , root_folder(root_folder) , map_path(map_path) , song_path(song_path) , song_preview_path(song_preview_path) , offset(offset)
{
    id = total_num_of_map;
    ++total_num_of_map;
}

Taiko_map::~Taiko_map()
{
    --total_num_of_map;
}
