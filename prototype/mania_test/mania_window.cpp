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
    //now i got a status bar, a menu, a black background...
    ui->setupUi(this);
    ui->gview->setScene(&thescene);
    ui->gview->show();

    //bound_rect_item = thescene.addRect(bound_rect); //this is the bounds
    //QRectF bounds = thescene.itemsBoundingRect();
    //ui->gview->fitInView(bounds, Qt::KeepAspectRatio);
    //QRectF weird = thescene.sceneRect();
    //qDebug() << weird.width() << " " << weird.height() << "weird" << endl;

    //ui->gview->fitInView(bound_rect, Qt::KeepAspectRatio); //dont use it...
    ui->gview->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    ui->actionaction_2->setVisible(false); //this is new toolbar version

    //set images, an obsolete part......
    //images[0] = ":/image/mania_test/images/small_square.png";
    //images[1] = ":/image/mania_test/images/minions.jpg";
    //try to add them
    //localitems[0] = thescene.addPixmap(QPixmap(images[0]));
    //localitems[0]->setOffset(100,100);
    //localitems[1] = thescene.addPixmap(QPixmap(images[1]));
    //localitems[1]->setOffset(200,200);
    //weird = thescene.sceneRect();
    //qDebug() << weird.width() << " " << weird.height() << "weird" << endl;

    //ui->gview->resize(500,300); it is resizing the view itself...
    ui->gview->scale(0.25,0.25); //worked, but not good though?
    bound_rect.setWidth(bound_rect.width()/0.25);
    bound_rect.setHeight(bound_rect.height()/0.25);
    //qDebug() << bound_rect.width() << " " << bound_rect.height() << "bound_weird" << endl;

    //connect clickable view to some function, however clickableview is for debug only.....
    connect(ui->gview, &clickable_view::mouseClicked, this, &mania_window::ui_view_clicked);

    //disable second button
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_4->setEnabled(false);

    //first i need to initialize the lanes, better use text file
    thelanes[0] = new lane(Qt::Key_D);
    thelanes[1] = new lane(Qt::Key_E);
    thelanes[2] = new lane(Qt::Key_S);
    thelanes[3] = new lane(Qt::Key_P);

    //now add the lanes to the scene
    for (int i=0; i<num_lanes; ++i){
        if (thelanes[i] == nullptr){
            qDebug() << "you foolish null pointer: lane " << i << endl;
            continue;
        }
        thelanes[i]->add_to_scene(i, num_lanes, &thescene, bound_rect); //scene can only pass by pointer/reference?
    }

    //now initialize tile_info_atlane since num_lanes is determined...
    for (int i=0; i<num_lanes; ++i) tile_info_atlane.append(QList<Tile_Adding_Info>());
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
    //setup static tiles, remember to connect them die and miss
    /*
    for (int i=0; i<num_lanes-1; ++i){
        NewTile* added_tile = thelanes[i]->addtile(lane::Add_Catagory::Normal, &thescene);
        if (added_tile != nullptr) connect(added_tile, &NewTile::die, this, &mania_window::missed_tile_response);
    }
    */
    //NewTile* added_tile = thelanes[num_lanes-1]->addtile(lane::Add_Catagory::Long, &thescene);
    //if (added_tile != nullptr && added_tile->get_tile_catagory() != NewTile::Tile_Catagory::Long) connect(added_tile, &NewTile::die, this, &mania_window::missed_tile_response);



    //set the buttons
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(false);

    //set the game_mode
    game_mode = Game_Mode::Play;
    //so now can parse the tiles
    parse_tiles(":/text/mania_test/beatmaps/mario_tiles.txt"); //this should be modified...


}
void mania_window::on_pushButton_2_clicked()
{
    //now add a timer to it
    global_timer = new QTimer;
    connect(global_timer, &QTimer::timeout, this, &mania_window::update);
    global_timer->start(refresh_rate);

    //lets play the music and see coherence...
    music_player = new QMediaPlayer();
    //music_player->setPlaylist(play_list);
    music_player->setMedia(QUrl("qrc:/sound_effect/mania_test/music/super_mario.mp3"));
    //music_player->setMedia(QUrl("qrc:/sound_effect/mania_test/music/super_mario_full.mp3"));
    music_player->play();
    connect(music_player, &QMediaPlayer::stateChanged, this, &mania_window::stop_timer);

    //disable itself
    ui->pushButton_2->setEnabled(false);
}

void mania_window::keyPressEvent(QKeyEvent *event){
    //qDebug() << "keypressed" << endl;
    //(event->isAutoRepeat)
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


}

void mania_window::keyReleaseEvent(QKeyEvent *event){
    if (!event->isAutoRepeat()){
        for (int i=0; i<num_lanes; ++i){
            if (event->key() == thelanes[i]->getkeynum() ){
                qDebug() << "lane " << i << "is released." << endl;

                if (game_mode == mania_window::Creative){
                    creative_addline(i);

                }
                thelanes[i]->on_key_released(&thescene);
                return;
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
        /* this worked, but for testing purposes only...
        if (timeelasped_ms % 1000 == 0){
            int x = QRandomGenerator::global()->bounded(num_lanes);
            int y = QRandomGenerator::global()->bounded(2);

            if (thelanes[x]->is_tile_list_empty()){
                NewTile* added_tile;
                if (y == 0) added_tile = thelanes[x]->addtile(lane::Add_Catagory::Long, &thescene);
                else added_tile = thelanes[x]->addtile(lane::Add_Catagory::Normal, &thescene);
                if (added_tile != nullptr && added_tile->get_tile_catagory() != NewTile::Tile_Catagory::Long) connect(added_tile, &NewTile::die, this, &mania_window::missed_tile_response);
            }

            //global_timer->stop();
        }
        */
        for (int i=0; i<num_lanes; ++i){
            if (tile_info_atlane[i].empty()) continue;
            Tile_Adding_Info info = tile_info_atlane[i][0];
            if ((timeelaseped_sec- info.time_to_add_sec)*1000 + (timeelasped_ms-info.time_to_add_ms) >= 0){
                NewTile* added_tile;
                if (info.catagory == lane::Add_Catagory::Normal){
                    added_tile = thelanes[i]->addtile(lane::Add_Catagory::Normal, &thescene);
                    connect(added_tile, &NewTile::die, this, &mania_window::missed_tile_response);
                }else if (info.catagory == lane::Add_Catagory::Long){
                    added_tile = thelanes[i]->addtile(lane::Add_Catagory::Long, &thescene);
                }else{
                    qDebug() << "noob nullptr" << endl;
                }
                tile_info_atlane[i].removeFirst();
                if (!tile_info_atlane[i].empty()) tile_info_atlane[i].removeFirst();
                //playing half notes only for now is deliberate otherwise it is too difficult for me...
             }
        }
    }

    for (int i=0; i<num_lanes; ++i){ //judge can be used for both creation and playing
        judge_response( thelanes[i]->update(&thescene));
    }
}

void mania_window::judge_response(lane::Judge_result result){
    if (result == lane::Judge_result::Good){
        combo++;
        score+= (0.05*combo + 1)*300;
    }else if (result == lane::Judge_result::Missed_Long_Tile){
        combo=0;
    }else if (result == lane::Judge_result::Continuing_Long_Tile){
        combo++;
        score+= 60;
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
    //score-= 200;
    ui->lcd_combo->display(combo);
    ui->lcd_score->display(score);
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
    connect(music_player, &QMediaPlayer::stateChanged, this, &mania_window::close_output);



    //disable itself then
    ui->pushButton_4->setEnabled(false);
}

void mania_window::time_update(){
    timeelasped_ms += refresh_rate;
    if (timeelasped_ms >= 1000){
        timeelaseped_sec++;
        timeelasped_ms -=1000;
    }
    ui->lcd_time->display(timeelaseped_sec);
}

void mania_window::close_output(){
    if (output != nullptr){
        output->close();
    }
    qDebug() << "output closed" << endl;
    stop_timer();
}

void mania_window::parse_tiles(QString file_name){
    input = new QFile(file_name);
    if (!input->open(QIODevice::ReadOnly)){
        qDebug() << "stupid, can't read/write" << endl;
        return;
    }
    in_stream = new QTextStream(input);

    QString line;
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
        qDebug() <<this_info.catagory <<" " << this_info.lane_num << " " << this_info.time_to_add_sec <<" "<<
                 this_info.time_to_add_ms << " " << this_info.long_cycle << " " << endl;

        //now add this to the qlist
        //first do modifications to offset the falling time
        int reduced_time = thelanes[this_info.lane_num]->getfall_time()-120;
        //this 120 offset is my mistake in creation.....
        this_info.time_to_add_ms -= reduced_time;
        while (this_info.time_to_add_ms < 0){
            this_info.time_to_add_ms += 1000;
            this_info.time_to_add_sec -=1;
        }
        tile_info_atlane[this_info.lane_num].append(this_info);
    }
    input->close();
}

void mania_window::stop_timer(){
    if (global_timer != nullptr){
        global_timer->stop();
    }
}
