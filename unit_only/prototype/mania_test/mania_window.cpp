#include <fstream>
using std::ifstream;
using std::noskipws;
using std::ws;
#include <iostream>
using std::cout;
using std::endl;

#include <QDebug>
#include "mania_window.h"
#include "ui_mania_window.h"
#include "clickable_view.h"

mania_window::mania_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mania_window)
{
    // solve qrc problems like read-only->create our own folder :) //done
    if (!QDir(folder_for_all).exists()) QDir().mkdir(folder_for_all);

    //now i got a status bar, a menu, a black background...
    ui->setupUi(this);
    ui->gview->setScene(&thescene);
    ui->gview->show();

    //ui->gview->fitInView(bound_rect, Qt::KeepAspectRatio); //dont use it...
    ui->gview->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    ui->actionaction_2->setVisible(false); //this is new toolbar version

    //set images, an obsolete part......
    //images[1] = ":/image/mania_test/images/minions.jpg";
    //localitems[1] = thescene.addPixmap(QPixmap(images[1]));
    //localitems[1]->setOffset(200,-200);

    //ui->gview->resize(500,300); it is resizing the view itself...
    ui->gview->scale(1/viewtoscene_scale, 1/viewtoscene_scale); //worked, but not good though?
    bound_rect.setWidth(bound_rect.width()*viewtoscene_scale);
    bound_rect.setHeight(bound_rect.height()*viewtoscene_scale);

    //connect clickable view to some function, however clickableview is for debug only.....
    connect(ui->gview, &clickable_view::mouseClicked, this, &mania_window::ui_view_clicked);

    //disable second button
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_4->setEnabled(false);

    //now setup the info of song+beatmap bundles, and setup the tree for display
    connect(ui->treeWidget, &QTreeWidget::itemPressed, this, &mania_window::on_treeWidget_itemClicked);
    setup_info(folder_for_all + info_file_name);

    //setup keys for the lanes
    setup_lanekeys(folder_for_all+"/data/lanes.txt");

    //QDir dir( QGuiApplication::applicationDirPath() );
            //dir.cd( "C:/DotaClient" );
            //QDir::setSearchPaths( "qml", QStringList( dir.absolutePath() ) );
    //QKeySequence seq = QKeySequence("D");
    //qDebug() << seq.count(); // 1
    //if (seq[0] == Qt::Key_D) qDebug() << "it is d."; // ok, worked

    /*
    QFileDialog dialog;
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Description (*.txt)"));
    //dialog.setSizePolicy(QSizePolicy::Fixed);
    dialog.setFixedSize(320,240);
    if (dialog.exec()) {
        QStringList fileNames = dialog.selectedFiles();
        qDebug() << fileNames[0] << endl;
    }
    */
}

mania_window::~mania_window()
{
    delete ui;
}

void mania_window::ui_view_clicked(int xpos, int ypos){
    qDebug() << "view coordinate: " <<xpos << " " << ypos << endl;
    QPointF scene_coor = ui->gview->mapToScene(xpos, ypos); //scenecoordinate...
    qDebug() << "scene coordinate: " << scene_coor.x() << " " << scene_coor.y() << endl;

}

void mania_window::on_pushButton_clicked()
{
    //stupid cases:)
    if (ui->label_4->text() == "Nothing") {
        qDebug() << "stupid, choose sth" << endl;
        return;
    }
    //set the buttons
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(false);

    //set the game_mode
    game_mode = Game_Mode::Play;

    //so now can parse the tiles and setup the playing envionment(lanes);
    //parse_tiles(":/text/mania_test/beatmaps/mario_tiles.txt"); //this should be modified...
    parse_tiles(folder_for_all+"/bundles"+current_beatmap_path);
    //now disable the treewidget
    disable_tree();
}
void mania_window::on_pushButton_2_clicked()
{
    qDebug() << current_beatmap_path << " is beatmap" << endl;
    qDebug() << current_song_path << " is song" << endl;

    //now add a timer to it
    global_timer = new QTimer;
    connect(global_timer, &QTimer::timeout, this, &mania_window::update);
    global_timer->start(refresh_rate);
    //original_time = previous_time = clock();
    original_time = previous_time = std::chrono::system_clock::now();

    //lets play the music and see coherence...
    music_player = new QMediaPlayer();
    //music_player->setPlaylist(play_list);
    music_player->setMedia(QUrl(folder_for_all+"/bundles"+current_song_path));
    //music_player->setMedia(QUrl("qrc:/sound_effect/mania_test/music/super_mario_full.mp3"));
    music_player->play();
    connect(music_player, &QMediaPlayer::stateChanged, this, &mania_window::stop_timer);

    //disable itself
    ui->pushButton_2->setEnabled(false);
}
void mania_window::on_pushButton_3_clicked()
{
    game_mode = Creative;
    ui->pushButton->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(true);

    //now open the output and start to write
    output = new QFile("mario_tiles.txt");
    if (!output->open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "stupid, can't read/write" << endl;
        return;
    }
    out_stream = new QTextStream(output); //it means that i need to *outstream.....
    disable_tree();
}
void mania_window::on_pushButton_4_clicked()
{
    //add a timer
    global_timer = new QTimer();
    connect(global_timer, &QTimer::timeout, this, &mania_window::update);
    global_timer->start(refresh_rate);

    //load the music then play then creative

    //em what's the problem???
    //play_list = new QMediaPlaylist();
    //qDebug() << play_list->addMedia(QUrl(":/sound_effect/mania_test/music/super_mario.mp3")) << endl;
    //play_list->setPlaybackMode(QMediaPlaylist::Loop);
    if (game_mode != Game_Mode::Creative){
        qDebug() << "stupid, wrong game mode" << endl;
        return;
    }
    music_player = new QMediaPlayer();
    //music_player->setPlaylist(play_list);
    music_player->setMedia(QUrl("qrc:/sound_effect/mania_test/music/super_mario.mp3"));
    //music_player->setMedia(QUrl("qrc:/sound_effect/mania_test/music/super_mario_full.mp3"));
    music_player->play();
    connect(music_player, &QMediaPlayer::stateChanged, this, &mania_window::output_close);



    //disable itself then
    ui->pushButton_4->setEnabled(false);
}

void mania_window::keyPressEvent(QKeyEvent *event){
    //qDebug() << "keypressed" << endl;
    //(event->isAutoRepeat)
    if (game_mode == Play){
        if (!event->isAutoRepeat()){
            for (int i=0; i<num_lanes; ++i){
                if (event->key() == thelanes[i]->getkeynum() ){
                        qDebug() << "lane " << i << "is pressed." << endl;
                        judge_response(thelanes[i]->on_key_pressed(&thescene, timeelaseped_sec,timeelasped_ms));

                    if (game_mode == mania_window::Creative){
                        //nothing else i think
                    }
                    return;
                }
            }
        }
    }else{
        //nothing
    }

}
void mania_window::keyReleaseEvent(QKeyEvent *event){
    if (game_mode == Play){
        if (!event->isAutoRepeat()){
            //debug
            qDebug() << "real time??? " <<real_time_elasped << endl;
            for (int i=0; i<num_lanes; ++i){
                if (event->key() == thelanes[i]->getkeynum() ){
                    //qDebug() << "lane " << i << "is released." << endl;

                    if (game_mode == mania_window::Creative){
                        creative_addline(i);

                    }
                    thelanes[i]->on_key_released(&thescene);
                    return;
                }
            }

        }
    }
}
void mania_window::creative_addline(int lane_num){
    int pressed_time = thelanes[lane_num]->getlongpress_time();
    pressed_time -= pressed_time % 40;
    int st_sec, st_ms;
    thelanes[lane_num]->getlastpress_tme(st_sec, st_ms);
    st_ms = st_ms-st_ms%50;

    if (thelanes[lane_num]->getlongpress_time() >= 100){ //a real long_press
        //qDebug() << "lane " << lane_num << "is long pressed." << thelanes[i]->getlongpress_time() << endl;
        *out_stream << "[long_tile]" << "[lane_num][" << lane_num << "][st_sec][" << st_sec <<
                       "][st_ms][" << st_ms << "][num_upadtes][" << pressed_time << "]" << endl;

    }else{ //a short press
        //qDebug() << "lane " << lane_num << "is short pressed." << thelanes[i]->getlongpress_time() << endl;
        *out_stream << "[normal_tile]" << "[lane_num][" << lane_num << "][st_sec][" << st_sec <<
                       "][st_ms][" << st_ms << "]" << endl;
    }
}

void mania_window::update(){
    time_update();


    if (game_mode == mania_window::Game_Mode::Play){
        for (int i=0; i<num_lanes; ++i){
            if (tile_info_atlane[i].empty()) continue;
            Tile_Adding_Info info = tile_info_atlane[i][0];
            //if ((timeelaseped_sec- info.time_to_add_sec)*1000 + (timeelasped_ms-info.time_to_add_ms) >= 0){
            if (real_time_elasped >= info.real_time_to_add){
                NewTile* added_tile;
                if (info.catagory == lane::Add_Catagory::Normal){
                    added_tile = thelanes[i]->addtile(lane::Add_Catagory::Normal, &thescene, info.long_cycle);
                    connect(added_tile, &NewTile::die, this, &mania_window::missed_tile_response);
                }else if (info.catagory == lane::Add_Catagory::Long){
                    added_tile = thelanes[i]->addtile(lane::Add_Catagory::Long, &thescene, info.long_cycle);
                }else{
                    qDebug() << "noob nullptr" << endl;
                }
                tile_info_atlane[i].removeFirst();
                //if (!tile_info_atlane[i].empty()) tile_info_atlane[i].removeFirst();
                //playing half notes only for now is deliberate otherwise it is too difficult for me...
             }
        }
    }
    //clock_t now =clock();
    //double time_used = ((double)(now-previous_time)/ CLOCKS_PER_SEC)*1000;
    //previous_time = now; //this is to ensure that the error won't add up...???

    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = now-previous_time;
    double time_used = elapsed_seconds.count()*1000;
    previous_time = now;

    for (int i=0; i<num_lanes; ++i){ //judge can be used for both creation and playing
        judge_response( thelanes[i]->update(&thescene, time_used/refresh_rate));
    }
}
void mania_window::time_update(){
    //clock_t now = clock();
    //real_time_elasped = ((double)(now-original_time)/ CLOCKS_PER_SEC)*1000;
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds= now-original_time;
    real_time_elasped = elapsed_seconds.count()*1000;

    timeelasped_ms += refresh_rate;
    if (timeelasped_ms >= 1000){
        timeelaseped_sec++;
        timeelasped_ms -=1000;
    }
    //ui->lcd_time->display(timeelaseped_sec);
    ui->lcd_time->display((int)real_time_elasped/1000);
}
void mania_window::judge_response(lane::Judge_result result){
    if (result == lane::Judge_result::Good){
        combo++;
        score+= (0.05*combo + 1)*300;
    }else if (result == lane::Judge_result::Missed_Long_Tile){
        combo=0;
    }else if (result == lane::Judge_result::Continuing_Long_Tile){
        combo++;
        //score+= 60;
    }else if (result == lane::Judge_result::Empty){
        //nothing happen
    }else if (result == lane::Judge_result::Undefined){
        qDebug() << "noob undefined la" << endl;
    }

    ui->lcd_combo->display(combo);
    ui->lcd_score->display(score);
}
void mania_window::missed_tile_response(){
    combo=0;
    ui->lcd_combo->display(combo);
    //ui->lcd_score->display(score);
}
void mania_window::stop_timer(){
    if (global_timer != nullptr){
        global_timer->stop();
    }
}

bool mania_window::input_standard_begin(QString file_name){
    input = new QFile(file_name);
    //input->setFileName(file_name);
    if (!input->open(QIODevice::ReadOnly)){
        qDebug() << "stupid, can't read/write:" << file_name << endl;
        return false;
    }
    in_stream = new QTextStream(input);
    return true;
}
void mania_window::input_standard_end(){
    input->close();
    delete input;
    delete in_stream;
}
void mania_window::output_close(){
    if (output != nullptr){
        output->close();
    }
    qDebug() << "output closed" << endl;
    stop_timer();
}

void mania_window::parse_tiles(QString file_name){
    if (!input_standard_begin(file_name)) return;

    QString line="";

    if (file_name.right(4) == ".txt"){
        in_stream->readLineInto(&line);
        num_lanes = line.toInt();
        for (int i=0; i<num_lanes; ++i){
            thelanes[i] = new lane(lane_keys[i], folder_for_all);
            thelanes[i]->add_to_scene(i, num_lanes, &thescene, bound_rect);
            tile_info_atlane.append(QList<Tile_Adding_Info>());
        }
        while (in_stream->readLineInto(&line)) {
            //qDebug() << line << endl;
            int left=0, right=0; //idk any better method......

            Tile_Adding_Info this_info; //get the info into the list and we are done

            while (line[right] != ']') right++;
            if (line.mid(left,right-left+1) == "[normal_tile]") this_info.catagory = lane::Add_Catagory::Normal;
            else if (line.mid(left,right-left+1) == "[long_tile]") this_info.catagory = lane::Add_Catagory::Long;
            //qDebug() << line.mid(left,right-left+1) << endl; //......
            right++; left=right; //transition

            while (line[right] != ']') right++; //[lane], useless
            right++; left=right;
            while (line[right] != ']') right++; //lane_num
            int temp = line.mid(left+1,right-left-1).toInt();
            this_info.lane_num = temp;
            right++; left=right; //transition
            //qDebug() << temp << "lane_num" << endl; //good

            while (line[right] != ']') right++; //[st_sec], useless
            right++; left=right;
            while (line[right] != ']') right++; //st_sec
            this_info.time_to_add_sec = line.mid(left+1,right-left-1).toInt();
            right++; left=right;

            while (line[right] != ']') right++; //[st_ms], useless
            right++; left=right;
            while (line[right] != ']') right++; //st_ms
            this_info.time_to_add_ms = line.mid(left+1,right-left-1).toInt();
            right++; left=right;

            if (this_info.catagory == lane::Add_Catagory::Long){
                while (line[right] != ']') right++; //[st_ms], useless
                right++; left=right;
                while (line[right] != ']') right++; //st_ms
                this_info.long_cycle = line.mid(left+1,right-left-1).toInt();
                right++; left=right;
            }
            //qDebug() <<this_info.catagory <<" " << this_info.lane_num << " " << this_info.time_to_add_sec <<" "<<
                     //this_info.time_to_add_ms << " " << this_info.long_cycle << " " << endl;

            //now add this to the qlist
            //first do modifications to offset the falling time
            int reduced_time = thelanes[this_info.lane_num]->getfalling_time()-120;
            //this 120 offset is my mistake in creation.....
            this_info.time_to_add_ms -= reduced_time;
            while (this_info.time_to_add_ms < 0){
                this_info.time_to_add_ms += 1000;
                this_info.time_to_add_sec -=1;
            }
            this_info.real_time_to_add = 1000*this_info.time_to_add_sec+this_info.time_to_add_ms;
            tile_info_atlane[this_info.lane_num].append(this_info);
        }
    }
    else if (file_name.right(4) == ".osu"){
        /* dummy implementaion only
        num_lanes = 1;
        thelanes[0] = new lane(lane_keys[0], folder_for_all);
        thelanes[0]->add_to_scene(0, num_lanes, &thescene, bound_rect);
        tile_info_atlane.append(QList<Tile_Adding_Info>());
        Tile_Adding_Info this_info;
        tile_info_atlane[0].append(this_info);
        */

        //first time parse the lanes, next time parse the tiles
        QMap<int, int> map;
        QSet<int> x_cor_set;
        QRegExp rx_comma("[,]"), rx_colon("[:]");// match a comma/colon
        while (line != "[HitObjects]") in_stream->readLineInto(&line); //unrelated
        while (in_stream->readLineInto(&line)){//the lines with numbers begin
            QStringList list = line.split(rx_comma, QString::SkipEmptyParts);
            //qDebug() << list << endl;
            x_cor_set.insert(list.at(0).toInt());
        }
        //qDebug() << x_cor_set;
        QList<int> cor_list = x_cor_set.toList();
        std::sort(cor_list.begin(), cor_list.end());
        for (int i=0; i<cor_list.size(); ++i){
            map.insert(cor_list[i], i);
            //qDebug() << map.value(cor_list[i]) << "from map value " <<  cor_list[i] << endl;
        }

        //now we put all the lanes into the game since lane_num is determined now
        num_lanes = cor_list.size();
        for (int i=0; i<num_lanes; ++i){
            thelanes[i] = new lane(lane_keys[i], folder_for_all);
            thelanes[i]->add_to_scene(i, num_lanes, &thescene, bound_rect);
            tile_info_atlane.append(QList<Tile_Adding_Info>());
        }

        //now go to second time to parse the tiles
        input_standard_end();
        if (!input_standard_begin(file_name)) return;
        line = "";
        while (line != "[HitObjects]") in_stream->readLineInto(&line); //unrelated
        //in_stream->readLineInto(&line); //for simplicity, now skip the first tile...
        while (in_stream->readLineInto(&line)){//the lines with numbers begin
            QStringList list = line.split(rx_comma, QString::SkipEmptyParts);

            int lane_num = map.value(list.at(0).toInt());
            QString colonstr = list.at(list.size()-1);
            QStringList remaining = colonstr.split(rx_colon, QString::SkipEmptyParts);

            Tile_Adding_Info this_info;
            int reduced_time = list.at(2).toInt() - thelanes[lane_num]->getfalling_time();

            this_info.lane_num = lane_num;
            this_info.time_to_add_sec = reduced_time/1000;
            this_info.time_to_add_ms = reduced_time%1000;
            this_info.real_time_to_add = reduced_time;

            if (remaining.size() > 4){ //mania longtile would get a endtime before that
                this_info.catagory = lane::Add_Catagory::Long;
                this_info.long_cycle = (remaining.at(0).toInt() - list.at(2).toInt()); //adhoc...
            }else if (remaining.size() == 4){ //osu norm, 0:0:0:0
                this_info.catagory = lane::Add_Catagory::Normal;
            }
            tile_info_atlane[lane_num].append(this_info);
        }
    }

    input_standard_end();
}
void mania_window::setup_lanekeys(QString file_name){
    if (!input_standard_begin(file_name)) return;

    QString line;
    in_stream->readLineInto(&line);
    int keycount = line.toInt();

    for (int i=0; i<keycount; ++i){
        in_stream->readLineInto(&line);
        QKeySequence seq = QKeySequence(line);
        lane_keys[i] = seq[0];
    }
    input_standard_end();
}

void mania_window::setup_info(QString file_name){
    if (!input_standard_begin(file_name)) return;

    QString line;
    in_stream->readLineInto(&line);
    num_songs = line.toInt();

    for (int i=0; i<num_songs; ++i){
        QTreeWidgetItem *added_item = new QTreeWidgetItem(ui->treeWidget);
        in_stream->readLineInto(&line); //name
        added_item->setText(0, line); //tr("cities") in exmple...

        in_stream->readLineInto(&line); //text file
        beatmap_paths.append(line);
        in_stream->readLineInto(&line); //audio file
        song_paths.append(line);
        in_stream->readLineInto(&line); //difficuly
        added_item->setText(1, line);
        tree_items.append(added_item);
    }

    input_standard_end();
}
void mania_window::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column){
    if (item->isDisabled()) return;
    int topnum = ui->treeWidget->indexOfTopLevelItem(item);
    qDebug() << "row: " << topnum << endl;
    ui->label_4->setText(item->text(0));
    current_song_path = song_paths[topnum];
    current_beatmap_path = beatmap_paths[topnum];
    //column = 0; //don't blame me:)
}
void mania_window::disable_tree(){
    for (int i=0; i<tree_items.size(); ++i){
        tree_items[i]->setDisabled(true);
    }
    ui->treeWidget->setFocusPolicy(Qt::NoFocus);
    ui->treeWidget->setAttribute(Qt::WA_ShowWithoutActivating);
}
