#ifndef MANIA_WINDOW_H
#define MANIA_WINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QRectF>
#include <QKeyEvent>
#include <QTimer>
#include <QRandomGenerator>
#include "lane.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTreeWidget>
#include <QFileDialog>
#include <ctime>
#include <chrono>
#include <QLabel>
#include <QElapsedTimer>

namespace Ui {
class mania_window;
}

class mania_window : public QMainWindow
{
    Q_OBJECT

public:
    enum Game_Mode{Undefined, Play, Creative};
    enum Game_Status{Waiting, Progressing, Paused};
    explicit mania_window(QWidget *parent = nullptr);
    ~mania_window();

private slots:
    void ui_view_clicked(int xpos, int ypos);

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void update();
    void missed_tile_response();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void time_update();

    void output_close();
    void start_timers();
    void stop_timer();
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void debug_only();

    void on_pushButton_5_clicked();

private:
    void set_initial_conditions();
    void judge_response(lane::Judge_result);

    void pause(); //not implemented now...
    void resume();
    void parse_tiles(QString file_name);
    void creative_addline(int lane_num);
    void setup_lanekeys(QString);
    void setup_info(QString);
    bool input_standard_begin(QString);
    void input_standard_end();
    void disable_tree();
    void label_set_adjust(QLabel *label, QString newtext);
    //void update_events();

    Ui::mania_window *ui;
    QGraphicsScene thescene{0,0,0,0};

    QRectF bound_rect{0,0,400,600}; //just a bound to the graphicsview
    const qreal viewtoscene_scale{4}; //1 pixel on the view = 4 pixels in the scene
    //QGraphicsRectItem *bound_rect_item; //a pointer to bounding rect, but should beuseless
    //everything should stay within this rect, aspect ratio should be 600:400 anyway

    //game_mode and stage related
    bool ispaused{false};
    Game_Mode game_mode{Undefined};
    Game_Status game_status{Waiting};

    //local images, debug only
    QGraphicsPixmapItem *localitems[100]; //pointers to temp local items
    QString images[100]; //useless
    //local texts for guiding the user
    QGraphicsSimpleTextItem* simple_text_item{nullptr};

    //the timer
    QTimer *global_timer{nullptr}, *global_timer2{nullptr};
    int refresh_rate{1};
    //double timeelasped_ms{0}, timeelaseped_sec{0}, real_time_elasped{0}; //dont use int and use ctime afu..
    //clock_t original_time,previous_time;
    //std::chrono::time_point<std::chrono::system_clock> original_time,previous_time;
    qint64 qtime_previous{0}, qtime_now{0}, qtime_elapsed{0}, qtime_paused{0};
    QElapsedTimer *elapsed_timer{nullptr}, *pause_timer{nullptr};

    //score calculation
    int combo{0} ,score{0};

    //now media_player and read/write files
    QMediaPlayer *music_player{nullptr};
    QMediaPlaylist *play_list{nullptr};
    QFile *input{nullptr}, *output{nullptr}; //now i try to abandon *input...
    QTextStream *in_stream{nullptr}, *out_stream{nullptr};

    //the lanes
    int num_lanes{4};
    lane *thelanes[100];
    QString lane_file_name;
    int lane_keys[100];//comes from a hard_coded file since don't want sb to upload himself
    struct Tile_Adding_Info{
        lane::Add_Catagory catagory{lane::Add_Catagory::Normal};
        int lane_num{0};
        int time_to_add_sec{3}, time_to_add_ms{0};
        double real_time_to_add{0.0};
        int long_cycle{-1};
    };
    QList<QList<Tile_Adding_Info>> tile_info_atlane; //this would be initialized by parser in mania_window...

    //should i use treewidget for choosing songs...
    int num_songs{0};
    QList<QTreeWidgetItem*> tree_items;
    QList<QString> song_paths, beatmap_paths;
    QString current_song_path{"/super_mario.mp3"}, current_beatmap_path{"/mario_tiles.txt"};
    QString info_file_name{"/bundles/songs_and_maps.txt"};

    //Own folder
    const QString folder_for_all{"c:/orz/AppData"}; //convention: '/' is left-associative
};

#endif // MANIA_WINDOW_H
