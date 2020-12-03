#include "map_selection_window.h"
#include "ui_map_selection_window.h"

#include <QDirIterator>
#include <mainwindow.h>
#include <QDebug>
#include <fstream>
#include <string>
#include <QUrl>
#include <QMediaPlaylist>

// init the number of maps
unsigned int Taiko_map::total_num_of_map = 0;

map_selection_window::map_selection_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::map_selection_window)
{
    ui->setupUi(this);

    music_player = new QMediaPlayer();

    this->setFixedSize(this->size());

    btn_inactive_style = "QPushButton { \
            font-size: 16px;\
font: bold large Aerial;\
color:rgb(17,17,17);\
    border-width: 1px;\
    border-radius: 6px;\
    border-bottom-color: rgb(150,150,150);\
    border-right-color: rgb(165,165,165);\
    border-left-color: rgb(165,165,165);\
    border-top-color: rgb(180,180,180);\
    border-style: solid;\
padding: 4px;\
    background-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(255, 255, 255, 255));\
}";

btn_active_style = "QPushButton{\
        font-size: 16px;\
font: bold large Aerial;\
color:rgb(17,17,17);\
border-width: 2px;\
border-radius: 6px;\
border-top-color: rgb(76,195,255);\
border-right-color: qlineargradient(spread:pad, x1:0, y1:1, x2:1, y2:0, stop:0 rgb(76,195,255), stop:1 rgb(76,195,255));\
border-left-color:  qlineargradient(spread:pad, x1:1, y1:0, x2:0, y2:0, stop:0 rgb(76,195,255), stop:1 rgb(76,195,255));\
border-bottom-color: rgb(76,195,255);\
border-style: solid;\
padding: 4px;\
background-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(255, 255, 255, 255));\
}";
}

map_selection_window::~map_selection_window()
{
    delete ui;
}


void map_selection_window::showEvent(QShowEvent *event)
{
    // load the image for back button
    QPixmap pixmap(":/image/image/back_button.png");
    QIcon ButtonIcon(pixmap);
    ui->btn_back->setIcon(ButtonIcon);
    ui->btn_back->setIconSize(pixmap.rect().size()/25);

    QDirIterator it("orz", QStringList() << "*.orz", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()){
        add_map(it.next());
    }
}

void map_selection_window::on_btn_auto_mode_clicked()
{
    if(!auto_mode){
        ui->btn_auto_mode->setStyleSheet(btn_active_style);
    }else{
        ui->btn_auto_mode->setStyleSheet(btn_inactive_style);
    }
    auto_mode = !auto_mode;
}

void map_selection_window::on_btn_high_speed_mode_clicked()
{
    if(!high_speed_mode){
        ui->btn_high_speed_mode->setStyleSheet(btn_active_style);
    }else{
        ui->btn_high_speed_mode->setStyleSheet(btn_inactive_style);
    }
    high_speed_mode = !high_speed_mode;
}

void map_selection_window::on_btn_dark_mode_clicked()
{
    if(!dark_mode){
        ui->btn_dark_mode->setStyleSheet(btn_active_style);
    }else{
        ui->btn_dark_mode->setStyleSheet(btn_inactive_style);
    }
    dark_mode = !dark_mode;
}

void map_selection_window::on_btn_fade_out_mode_clicked()
{
    if(!fade_out_mode){
        ui->btn_fade_out_mode->setStyleSheet(btn_active_style);
    }else{
        ui->btn_fade_out_mode->setStyleSheet(btn_inactive_style);
    }
    fade_out_mode = !fade_out_mode;
}

void map_selection_window::on_btn_random_mode_clicked()
{
    if(!random_mode){
        ui->btn_random_mode->setStyleSheet(btn_active_style);
    }else{
        ui->btn_random_mode->setStyleSheet(btn_inactive_style);
    }
    random_mode = !random_mode;
}

void map_selection_window::on_btn_back_clicked()
{
    close();
}

void map_selection_window::closeEvent(QCloseEvent *)
{
    MainWindow* w = new MainWindow();
    w->show();
    close();
    delete music_player;
}

void map_selection_window::add_map(QString map_path)
{
    // create a stringlist for creating QTreeWidgetItem later
    QStringList info_list;

    // use to get the paht of the root folder
    QFileInfo info(map_path);

    // read the file
    std::ifstream map(map_path.toStdString());

    // read all the map information and save them to a temp variable;
    std::string map_name;
    map >> map_name;
    info_list.append(map_name.c_str());

    std::string duration;
    map >> duration;
    info_list.append(duration.c_str());

    std::string creator;
    map >> creator;
    info_list.append(creator.c_str());

    std::string difficulty;
    map >> difficulty;
    info_list.append(difficulty.c_str());

    // assume the song is put in the same folder
    std::string song_name;
    map >> song_name;
    std::string song_path;
    song_path = info.dir().path().toStdString() + "/" + song_name;

    int offset;
    map >> offset;

    // assume the preview song is called "preview.mp3" and put in the same folder
    std::string song_preview_path;
    song_preview_path = info.dir().path().toStdString() + "/preview.mp3";

    // create a map and put it into the list
    Taiko_map* taiko_map= new Taiko_map(map_name.c_str() , duration.c_str() , creator.c_str() , difficulty.c_str() , info.dir().path() , map_path , song_path.c_str() , song_preview_path.c_str() , offset);
    map_list.append(taiko_map);

    info_list.append(QString::number(taiko_map->id));

    QTreeWidgetItem* infos = new QTreeWidgetItem(info_list);

    ui->map_tree->addTopLevelItem(infos);

}

void map_selection_window::on_map_tree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    // stop the music first
    music_player->stop();
    // get the map by ID ( id is same as the index of list )
    Taiko_map* map = map_list.at(item->data(4,0).toInt());
    QString song_preview_path = map->song_preview_path;
    if(file_exist(song_preview_path)){

        // play the preview song of selected map and loop it
        QMediaPlaylist *playlist = new QMediaPlaylist();
        playlist->addMedia(QUrl(song_preview_path));
        playlist->setPlaybackMode(QMediaPlaylist::Loop);

        music_player->setPlaylist(playlist);
        music_player->play();
    }

    // clear the current score for showing the new score
    ui->score_tree->clear();

    // find the result file
    std::string result_path = map->root_folder.toStdString() + "/result.txt";
    if(!file_exist(result_path.c_str())){

        // create a text file "songlist"
        QFile file(result_path.c_str());
        if ( file.open(QIODevice::ReadWrite) )
        {
            // write the current song list into the file
            QTextStream stream( &file );
            stream << "" << endl;
        }

    }else{
        QFile result_file(result_path.c_str());
        QStringList result_row;
        if (result_file.open(QIODevice::ReadOnly))
        {
           QTextStream temp(&result_file);
           // read the file line by line and split each line by space
           while (!temp.atEnd())
           {
              result_row.append(temp.readLine().split(" "));
           }
           result_file.close();
           QTreeWidgetItem* infos = new QTreeWidgetItem(result_row);
           ui->score_tree->addTopLevelItem(infos);
        }
    }
}

bool map_selection_window::file_exist(QString path){
    QFileInfo check_file(path);
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}
