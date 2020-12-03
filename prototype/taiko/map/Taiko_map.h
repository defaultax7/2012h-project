#ifndef SONG_LIST_CONTROLLER_H
#define SONG_LIST_CONTROLLER_H

#include <QString>

class Taiko_map
{
public:
    Taiko_map(QString map_name , QString duration, QString creator, QString difficulty, QString map_path, QString song_path);
    const QString map_name;
    const QString duration;
    const QString creator;
    const QString difficulty;
    const QString map_path;
    const QString song_path;
};

#endif // SONG_LIST_CONTROLLER_H
