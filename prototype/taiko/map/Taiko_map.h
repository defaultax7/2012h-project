#ifndef SONG_LIST_CONTROLLER_H
#define SONG_LIST_CONTROLLER_H

#include <QString>

class Taiko_map
{
public:
    Taiko_map(QString map_name , QString duration, QString creator, QString difficulty, QString root_folder, QString map_path, QString song_path, QString song_preview_path , int offset);
    ~Taiko_map();
    const QString map_name;
    const QString duration;
    const QString creator;
    const QString difficulty;
    const QString root_folder;
    const QString map_path;
    const QString song_path;
    const QString song_preview_path;
    const int offset;
    unsigned int id;
    static unsigned int total_num_of_map;
};

#endif // SONG_LIST_CONTROLLER_H
