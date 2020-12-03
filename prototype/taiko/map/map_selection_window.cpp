#include "map_selection_window.h"
#include "ui_map_selection_window.h"

#include <QDirIterator>
#include <mainwindow.h>
#include <QDebug>
#include <fstream>
#include <string>
#include <QUrl>
#include <QMediaPlaylist>
#include <QMessageBox>

#include <taiko/taiko_window.h>

// init the number of maps
unsigned int Taiko_map::total_num_of_map = 0;

map_selection_window::map_selection_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::map_selection_window)
{

    ui->setupUi(this);

    music_player = new QMediaPlayer();

    this->setFixedSize(this->size());

    // enable sorting for both tree view
    QHeaderView* map_tree_header = ui->map_tree->header();
    map_tree_header->setSectionsClickable(true);
    connect(map_tree_header, SIGNAL(sectionClicked(int)), this, SLOT(sort_map_tree_by_header(int)));

    QHeaderView* score_tree_header = ui->score_tree->header();
    score_tree_header->setSectionsClickable(true);
    connect(score_tree_header, SIGNAL(sectionClicked(int)), this, SLOT(sort_score_tree_by_header(int)));

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


    //    qDebug() << ui->map_tree->header->sectionClicked();
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
    if(!started_game){
        MainWindow* w = new MainWindow();
        w->show();
    }
    for(int i = 0; i < map_list.count() ; ++i){
        // dont delete the selected map, it will be passed to the game mode
        delete map_list.at(i);
    }
    delete music_player;
    close();
}

void map_selection_window::add_map(QString map_path)
{
    // use to get the paht of the root folder
    QFileInfo info(map_path);

    // read the file
    std::ifstream map(map_path.toStdString());

    // read all the map information and save them to a temp variable ( they are used to create the taiko map object later )
    std::string map_name;
    map >> map_name;

    std::string duration;
    map >> duration;

    std::string creator;
    map >> creator;

    std::string difficulty;
    map >> difficulty;

    // assume the song is put in the same folder
    std::string song_name;
    map >> song_name;
    std::string song_path;
    song_path = info.dir().path().toStdString() + "/" + song_name;

    int offset;
    map >> offset;

    int note_left;
    map >> note_left;

    // assume the preview song is called "preview.mp3" and put in the same folder
    std::string song_preview_path;
    song_preview_path = info.dir().path().toStdString() + "/preview.mp3";

    // create a map and put it into the list
    Taiko_map* taiko_map= new Taiko_map(map_name.c_str() , duration.c_str() , creator.c_str() , difficulty.c_str() , info.dir().path() , map_path , song_path.c_str() , song_preview_path.c_str() , offset , note_left);
    map_list.append(taiko_map);
    add_map_to_tree(taiko_map);

}

void map_selection_window::on_map_tree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    // set the selected index and get the map by ID ( id is same as the index of list )
    selected_map_index = item->data(4,0).toInt();
    Taiko_map* map = map_list.at(selected_map_index);

    // update the selected map view
    ui->lb_selected_map->setText(map->map_name);

    // stop the music first
    music_player->stop();

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

void map_selection_window::on_btn_start_clicked()
{
    if(selected_map_index == -1){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("Please select a map first!");
        msgBox.exec();
    }else{
        started_game = true;
        taiko_window* w = new taiko_window(map_list.at(selected_map_index)->map_path , map_list.at(selected_map_index)->song_path , map_list.at(selected_map_index)->note_left , auto_mode , high_speed_mode , dark_mode , fade_out_mode , random_mode );
        w->show();
        close();
    }
}

void map_selection_window::reset_selected_song(){
    //    selected_map_index = -1;
    //    ui->score_tree->clear();
    ui->map_tree->clear();
}

void map_selection_window::add_map_to_tree(Taiko_map * map)
{
    // add one map to the map tree view
    QStringList temp;
    temp.append(map->map_name);
    temp.append(map->duration);
    temp.append(map->creator);
    temp.append(map->difficulty);
    temp.append(QString::number(map->id));
    QTreeWidgetItem* item = new QTreeWidgetItem(temp);
    ui->map_tree->addTopLevelItem(item);
}

void map_selection_window::refresh_filtered_list()
{
    reset_selected_song();
    for (int i = 0; i < map_list.count() ; ++i) {
        // filter difficulty first
        if(map_list.at(i)->difficulty == ui->comBox_difficulty->currentText() || ui->comBox_difficulty->currentText() == "Any"){
            // filter by map name
            if(ui->comboBox->currentIndex() == 0){
                if (map_list.at(i)->map_name.toStdString().find(ui->txt_search->text().toStdString()) != std::string::npos) {
                    add_map_to_tree(map_list.at(i));
                }
            }else if(ui->comboBox->currentIndex() == 1){
                // filter by creator nam
                if (map_list.at(i)->creator.toStdString().find(ui->txt_search->text().toStdString()) != std::string::npos) {
                    add_map_to_tree(map_list.at(i));
                }
            }
        }
    }
}

void map_selection_window::on_comBox_difficulty_currentIndexChanged(const QString &arg1)
{
    refresh_filtered_list();
}

void map_selection_window::on_comboBox_currentIndexChanged(int index)
{
    refresh_filtered_list();
}

void map_selection_window::on_txt_search_textChanged(const QString &arg1)
{
    refresh_filtered_list();
}

void map_selection_window::sort_map_tree_by_header(int header_index)
{
    ui->map_tree->sortByColumn(header_index, Qt::DescendingOrder);
}

void map_selection_window::sort_score_tree_by_header(int header_index)
{
    ui->score_tree->sortByColumn(header_index, Qt::DescendingOrder);
}
