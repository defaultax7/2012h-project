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

mania_window::mania_window(QWidget *parent) : QMainWindow(parent), ui(new Ui::mania_window) //constructor to setup everything
{
    // solve qrc problems like read-only->create our own folder :) //done
    if (!QDir(folder_for_all).exists()) QDir().mkdir(folder_for_all);

    //now i got a status bar, a menu, a black background...
    ui->setupUi(this);
    ui->gview->setScene(&thescene);
    ui->gview->show();

    ui->gview->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    ui->actionaction_2->setVisible(false); //this is new toolbar version
    ui->actionaction_editor1->setVisible(false); //give up on toolbar

    //setting up the view
    ui->gview->scale(1/viewtoscene_scale, 1/viewtoscene_scale); //worked, but not good though?
    bound_rect.setWidth(bound_rect.width()*viewtoscene_scale);
    bound_rect.setHeight(bound_rect.height()*viewtoscene_scale);

    //connect clickable view to some function, however clickableview is for debug only.....
    connect(ui->gview, &clickable_view::mouseClicked, this, &mania_window::ui_view_clicked);

    //about labels
    ui->label_piconly->setPixmap(QPixmap(folder_for_all+"/images/orz_mania.png"));

    //setup default keys for the lanes
    setup_lanekeys(folder_for_all+"/data/lanes.txt");

    //adhoc initializations of remaining pointers and combo box, spin box etc
    for (int i=0; i<100; ++i) thelanes[i] = nullptr;
    set_initial_conditions(); //this is the initial state before and after each playing/creation
    ui->spinBox->setMaximum(9); //now i allow 10 keys, but the scene also don't have place for it ...
    for (int i=0; i<26; ++i){
        char ch = 'A'+i;
        ui->comboBox->addItem(QString(ch));
    }
    ui->comboBox->setCurrentIndex(3);

    ui->spinBox_numcreate->setMinimum(2);
    ui->spinBox_numcreate->setMaximum(7);

}
void mania_window::set_initial_conditions(){//the initial conditions
    //gamemode
    game_mode = Undefined;
    game_status = Waiting;

    text_item = (thescene.addText(tr("Please do the key binding and select a song."), QFont("Times", 50, QFont::Bold)));
    text_item->setDefaultTextColor(Qt::blue);
    text_item->setPos(40,40);

    //buttons
    ui->pushButton->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);

    //time, timers are controlled by actions of buttons
    qtime_previous = qtime_now= qtime_elapsed= qtime_paused = 0;
    //combo score and related displays
    combo = score = 0;
    ui->lcd_combo->display(combo);
    ui->lcd_score->display(score);
    ui->lcd_time->display(0);

    //reset the treewidget
    ui->treeWidget->clear();
    tree_items.clear(); //clear the lines of pointer since they become useless:)
    connect(ui->treeWidget, &QTreeWidget::itemPressed, this, &mania_window::on_treeWidget_itemClicked);
    setup_info(folder_for_all + info_file_name);

    //spin combo boxes
    ui->comboBox->setEnabled(true);
    ui->spinBox->setEnabled(true);
}
mania_window::~mania_window()//delete the pointers
{
    delete ui;
    delete global_timer;
    delete global_timer2;
    delete elapsed_timer;
    delete pause_timer;
    delete music_player;
    //delete play_list;
    for (int i=0; i<num_lanes; ++i) delete thelanes[i];
}

void mania_window::ui_view_clicked(int xpos, int ypos){ //a debug function only
    qDebug() << "view coordinate: " <<xpos << " " << ypos << endl;
    QPointF scene_coor = ui->gview->mapToScene(xpos, ypos); //scenecoordinate...
    qDebug() << "scene coordinate: " << scene_coor.x() << " " << scene_coor.y() << endl;

}

void mania_window::on_pushButton_clicked() //setup for playing
{
    //stupid cases:)
    //fist ready to reset the error message
    if (warning_item != nullptr){
        thescene.removeItem(warning_item);
        warning_item = nullptr;
    }
    //then the cases
    if (ui->label_4->text() == "Nothing") {
        warning_item = thescene.addText(tr("Choose a map!"), QFont("Times", 50, QFont::Bold));
        warning_item->setDefaultTextColor(Qt::red);
        warning_item->setPos(40,140);
        return;
    }
    for (int i=0; i<num_lanes; ++i){
        for (int j=i+1; j<num_lanes; ++j){
            if (lane_keys[i] == lane_keys[j]){
                warning_item = thescene.addText(tr("Repeated keys for using lanes detected!"), QFont("Times", 50, QFont::Bold));
                warning_item->setDefaultTextColor(Qt::red);
                warning_item->setPos(40,140);
                return;
            }
        }
    }
    //remove text message
    if (text_item != nullptr){
        thescene.removeItem(text_item);
        text_item = nullptr;
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
    //now disable the treewidget and the combo spin check boxes
    disable_tree();
    ui->comboBox->setEnabled(false);
    ui->spinBox->setEnabled(false);
    ui->spinBox_numcreate->setEnabled(false);
}
void mania_window::on_pushButton_2_clicked() //begin the gameplay
{
    qDebug() << current_beatmap_path << " is beatmap" << endl;
    qDebug() << current_song_path << " is song" << endl;



    //now change to game_status
    game_status = Progressing;
    //now add add timers
    start_timers();

    //lets play the music and see coherence...
    if (music_player != nullptr) delete music_player;
    music_player = new QMediaPlayer();
    music_player->setVolume(50);
    music_player->setMedia(QUrl(folder_for_all+"/bundles"+current_song_path));
    music_player->play();
    connect(music_player, &QMediaPlayer::stateChanged, this, &mania_window::stop_progress);

    //disable itself
    ui->pushButton_2->setEnabled(false);
}
void mania_window::on_pushButton_3_clicked() //setup fro creation
{
    //remove text message
    if (text_item != nullptr){
        thescene.removeItem(text_item);
        text_item = nullptr;
    }

    game_mode = Creative;
    ui->pushButton->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(true);

    num_lanes = ui->spinBox_numcreate->value();
    for (int i=0; i<num_lanes; ++i){
        if (thelanes[i] != nullptr) delete thelanes[i];
        thelanes[i] = new lane(lane_keys[i], folder_for_all);
        thelanes[i]->add_to_scene(i, num_lanes, &thescene, bound_rect);
        tile_info_atlane.append(QList<Tile_Adding_Info>());;
    }
    output_standard("/trial.txt");
    //now disable the treewidget and the combo spin check boxes
    disable_tree();
    ui->comboBox->setEnabled(false);
    ui->spinBox->setEnabled(false);
    ui->spinBox_numcreate->setEnabled(false);

    //now add a dialog
    QFileDialog dialog;
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("music (*.mp3 *.wav)"));
    //dialog.setSizePolicy(QSizePolicy::Fixed);
    dialog.setFixedSize(320,240);
    if (dialog.exec()) {
        QStringList fileNames = dialog.selectedFiles();
        qDebug() << fileNames[0] << endl;
        current_song_path = fileNames[0];
    }

}
void mania_window::on_pushButton_4_clicked() //begin creation
{
    //add a timer
    start_timers();

    //load the music then play then creative
    if (game_mode != Game_Mode::Creative){
        qDebug() << "stupid, wrong game mode" << endl;
        return;
    }
    game_status = Progressing;

    if (music_player != nullptr) delete music_player;
    music_player = new QMediaPlayer();
    music_player->setVolume(50);

    if (current_song_path.at(0) == "/"){ //relative
        music_player->setMedia(QUrl(folder_for_all+"/bundles"+current_song_path));
    }else{ //absolute
        music_player->setMedia(QUrl(current_song_path));
    }

    music_player->play();
    connect(music_player, &QMediaPlayer::stateChanged, this, &mania_window::stop_progress);

    //disable itself then
    ui->pushButton_4->setEnabled(false);
}
void mania_window::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column){ //change the chosen song when tree is clicked
    if (item->isDisabled()) return;
    int topnum = ui->treeWidget->indexOfTopLevelItem(item);
    //qDebug() << "row: " << topnum << endl;

    //label:
    label_set_adjust(ui->label_4, item->text(0));

    //setup the paths
    current_song_path = song_paths[topnum];
    current_beatmap_path = beatmap_paths[topnum];
    //column = 0; //don't blame me:)
}

void mania_window::keyPressEvent(QKeyEvent *event){ //when you press keyboard
    //qDebug() << "keypressed" << endl;
    if (event->isAutoRepeat()) return;
    if (game_mode == Play){
        if (game_status == Progressing){
            if (!event->isAutoRepeat()){
                for (int i=0; i<num_lanes; ++i){
                    if (event->key() == thelanes[i]->getkeynum() ){
                            judge_response(thelanes[i]->on_key_pressed(&thescene, -1,-1)); //broken, use dummy instead
                    }
                }
                if (event->key() == Qt::Key_Escape){ //trying to pause the game
                    pause();
                }
            }
        }else if (game_status == Paused && event->key() == Qt::Key_Escape){
            resume();
        }
    }else if (game_mode == Creative){
        //slightly different
        if (game_status == Progressing){
            if (!event->isAutoRepeat()){
                for (int i=0; i<num_lanes; ++i){
                    if (event->key() == thelanes[i]->getkeynum() ){
                        judge_response(thelanes[i]->on_key_pressed(&thescene, qtime_elapsed/1000, qtime_elapsed%1000)); //broken, use dummy instead
                    }
                }
            }
        }
    }

}
void mania_window::keyReleaseEvent(QKeyEvent *event){ //when you released keyboard
    if (game_mode == Play && game_status == Progressing){
        if (!event->isAutoRepeat()){
            for (int i=0; i<num_lanes; ++i){
                if (event->key() == thelanes[i]->getkeynum() ){
                    thelanes[i]->on_key_released(&thescene);
                    return;
                }
            }
        }
    }else if (game_mode == Creative){
        if (game_status == Progressing){
            if (!event->isAutoRepeat()){
                for (int i=0; i<num_lanes; ++i){
                    if (event->key() == thelanes[i]->getkeynum() ){
                        creative_addline(i);
                        thelanes[i]->on_key_released(&thescene);
                        return;
                    }
                }
            }
        }
    }
}
void mania_window::creative_addline(int lane_num){ //to generate output in creative mode
    int pressed_time = thelanes[lane_num]->getlongpress_time();
    pressed_time -= pressed_time % 20;

    int st_sec, st_ms;
    thelanes[lane_num]->getlastpress_tme(st_sec, st_ms);
    st_ms = st_ms-st_ms%50;

    if (thelanes[lane_num]->getlongpress_time() >= 80){ //a real long_press
        //qDebug() << "lane " << lane_num << "is long pressed." << thelanes[i]->getlongpress_time() << endl;
        *out_stream << "[long_tile]" << "[lane_num][" << lane_num << "][st_sec][" << st_sec <<
                       "][st_ms][" << st_ms << "][num_upadtes][" << pressed_time << "]" << endl;

    }else{ //a short press
        //qDebug() << "lane " << lane_num << "is short pressed." << thelanes[i]->getlongpress_time() << endl;
        *out_stream << "[normal_tile]" << "[lane_num][" << lane_num << "][st_sec][" << st_sec <<
                       "][st_ms][" << st_ms << "]" << endl;
    }
}

void mania_window::update(){//update the status of the graphicsscene at some fixed interval
    time_update();
    //qDebug() << "wtf" << endl;

    if (game_mode == mania_window::Game_Mode::Play && game_status == Progressing){
        for (int i=0; i<num_lanes; ++i){
            if (tile_info_atlane[i].empty()) continue;
            Tile_Adding_Info info = tile_info_atlane[i][0];
            //if ((timeelaseped_sec- info.time_to_add_sec)*1000 + (timeelasped_ms-info.time_to_add_ms) >= 0){
            //if (real_time_elasped >= info.real_time_to_add){
            //qDebug() << elapsed_timer->elapsed() << info.time_to_add_sec*1000+info.time_to_add_ms << endl;
            //long used = elapsed_timer->elapsed();
            if (qtime_elapsed >= info.time_to_add_sec*1000+info.time_to_add_ms){
                //qDebug() << "true" << endl;
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
    qtime_now = elapsed_timer->elapsed()-qtime_paused;
    long time_used = qtime_now-qtime_previous;
    //qDebug() << time_used << endl;
    qtime_previous = qtime_now;

    for (int i=0; i<num_lanes; ++i){ //judge can be used for both creation and playing
        judge_response( thelanes[i]->update(&thescene, time_used/refresh_rate));
    }
}
void mania_window::time_update(){//just update the time
    //clock_t now = clock();
    //real_time_elasped = ((double)(now-original_time)/ CLOCKS_PER_SEC)*1000;
    //std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    //std::chrono::duration<double> elapsed_seconds= now-original_time;
    //real_time_elasped = elapsed_seconds.count()*1000;

    /*
    timeelasped_ms += refresh_rate;
    if (timeelasped_ms >= 1000){
        timeelaseped_sec++;
        timeelasped_ms -=1000;
    }
    */
    //ui->lcd_time->display(timeelaseped_sec);
    //ui->lcd_time->display((int)real_time_elasped/1000);
    qtime_elapsed = elapsed_timer->elapsed()-qtime_paused;
    ui->lcd_time->display((int)qtime_elapsed/1000);
}
void mania_window::judge_response(lane::Judge_result result){//passing the press/long press info to the lanes and they return the result
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
void mania_window::missed_tile_response(){//connected to when you missed a tile in a lane
    combo=0;
    ui->lcd_combo->display(combo);
    //ui->lcd_score->display(score);
}
void mania_window::start_timers(){ //timer controllers, begin them
    //cycle timer
    if (global_timer != nullptr) delete global_timer;
    global_timer = new QTimer;
    connect(global_timer, &QTimer::timeout, this, &mania_window::update);
    global_timer->start(refresh_rate);

    //original_time = previous_time = std::chrono::system_clock::now();
    qtime_previous = qtime_now = qtime_elapsed = 0;

    //elapsed timer
    if (elapsed_timer != nullptr) delete elapsed_timer;
    elapsed_timer = new QElapsedTimer;
    elapsed_timer->start();

}
void mania_window::stop_progress(){ //stop the play/creation progress
    if (game_mode == Play && game_status == Progressing){
        if (global_timer != nullptr){
            global_timer->stop();
        }
        ui->pushButton_5->setEnabled(true);
    }
    if (game_mode == Creative && game_status == Progressing){
        if (global_timer != nullptr){
            global_timer->stop();
        }
        ui->pushButton_5->setEnabled(true);
        output_close();
        qDebug() << "creation done" << endl;
    }
}

bool mania_window::input_standard_begin(QString file_name){//standard procedure with that file
    input = new QFile(file_name);
    //input->setFileName(file_name);
    if (!input->open(QIODevice::ReadOnly)){
        qDebug() << "stupid, can't read/write:" << file_name << endl;
        return false;
    }
    in_stream = new QTextStream(input);
    return true;
}
void mania_window::input_standard_end(){//standard procedure to end input
    input->close();
    delete input;
    delete in_stream;
}
bool mania_window::output_standard(QString file_name){ //output begin standards
    if (output != nullptr) delete output;
    output = new QFile(folder_for_all+"/outputs"+file_name);
    if (!output->open(QIODevice::WriteOnly | QIODevice::Text)){
        //qDebug() << "stupid, can't read/write" << endl;
        return false;
    }
    if (out_stream != nullptr) delete out_stream;
    out_stream = new QTextStream(output);

    if (game_mode == Creative) *out_stream << num_lanes << endl;
    return true;
}
void mania_window::output_close(){ //output end standards
    if (output != nullptr){
        output->close();
        output = nullptr;
    }
    if (out_stream != nullptr){
        delete  out_stream;
    }
    qDebug() << "output closed" << endl;

}

void mania_window::parse_tiles(QString file_name){ //parse the texttile and store the info of tiles locally
    if (!input_standard_begin(file_name)) return;

    QString line="";

    if (file_name.right(4) == ".txt"){
        in_stream->readLineInto(&line);
        num_lanes = line.toInt();
        for (int i=0; i<num_lanes; ++i){
            if (thelanes[i] != nullptr) delete thelanes[i];
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
            if (thelanes[i] != nullptr) delete  thelanes[i];
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
            if (reduced_time >=0) tile_info_atlane[lane_num].append(this_info); //i give up on too early tiles
        }
    }

    input_standard_end();
}
void mania_window::setup_lanekeys(QString file_name){ //just setup the default keys for the lanes
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

void mania_window::setup_info(QString file_name){ //setup the treewidget
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
void mania_window::disable_tree(){//to disable the tree
    for (int i=0; i<tree_items.size(); ++i){
        tree_items[i]->setDisabled(true);
    }
    ui->treeWidget->setFocusPolicy(Qt::NoFocus);
    ui->treeWidget->setAttribute(Qt::WA_ShowWithoutActivating);
}

void mania_window::label_set_adjust(QLabel *label, QString newtext){ //for auto resize of label text
    int fit = false;
    QFont myFont = label->font();
    myFont.setPointSize(16);
    while (!fit)
    {
        QFontMetrics fm( myFont );
        QRect bound = fm.boundingRect(0,0, label->width(), label->height(), Qt::TextWordWrap | Qt::AlignLeft, newtext);

        if (bound.width() <= label->width() &&
            bound.height() <= label->height())
            fit = true;
        else
            myFont.setPointSize(myFont.pointSize() - 1);
    }
    //qDebug() << myFont.pointSize() << " em?" << endl; so this code has error...
    label->setFont(myFont);
    label->setText(newtext);
}

void mania_window::pause(){ //for pauses in playing mode
    if (game_mode == Play && game_status == Progressing){
        game_status = Paused;
        if (global_timer != nullptr) global_timer->stop();
        music_player->pause();

        if (pause_timer == nullptr){
            pause_timer = new QElapsedTimer;
            pause_timer->start();
        }else{
            pause_timer->restart(); //no stoparrrrr
        }

        for (int i=0; i<num_lanes; ++i){
            if (thelanes[i]->getlongpress_time()>0)thelanes[i]->on_key_released(&thescene);
        }
        text_item = (thescene.addText(tr("Paused. You can drink water or go to toliet:)"), QFont("Times", 50, QFont::Bold)));
        text_item->setDefaultTextColor(Qt::yellow);
        text_item->setPos(40,40);
    }
}

void mania_window::resume(){ //return to the ongoing game in play mode
    if (game_mode == Play && game_status == Paused){
        music_player->play();
        game_status = Progressing;

        if (global_timer != nullptr){
            global_timer->stop();
            delete global_timer;
            global_timer = nullptr;
        }

        global_timer = new QTimer();
        connect(global_timer, &QTimer::timeout, this, &mania_window::update);
        global_timer->start(refresh_rate);

        qtime_paused += pause_timer->elapsed();

        thescene.removeItem(text_item);
        text_item = nullptr;
    }
}

void mania_window::debug_only(){ //debug only
    qDebug() << "i have a bug" << endl;
}

void mania_window::on_pushButton_5_clicked() //the button to get back to initial conditions
{
    set_initial_conditions();
}

void mania_window::on_spinBox_valueChanged(int arg1)//for key binding
{
    qDebug() << "i have a number: " << arg1 << endl;
    ui->comboBox->setCurrentIndex(ui->comboBox->findText(QKeySequence(lane_keys[arg1]).toString() ));
}

void mania_window::on_comboBox_currentTextChanged(const QString &arg1)//for key binding
{

    int lane_num = ui->spinBox->value();
    qDebug() << "i have another number: " << lane_num << endl;
    QKeySequence seq = QKeySequence(arg1);
    lane_keys[lane_num] = seq[0];
}
