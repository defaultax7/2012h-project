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
    void ui_view_clicked(int xpos, int ypos); //clickable view for debug only

    //the buttons, their names...
    void on_pushButton_clicked(); //setup playing
    void on_pushButton_2_clicked(); //begin the gameplay
    void on_pushButton_3_clicked(); //setup fro creation
    void on_pushButton_4_clicked(); //begin creation
    void on_pushButton_5_clicked(); //reset to initial conditions

    //keyboard
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    //related to falling tiles
    void update();
    void missed_tile_response();
    void time_update();

    //game_status and timers related together
    void start_timers();
    void stop_progress();

    //timely file io
    void output_close();

    //for setting up key binding and songs
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_spinBox_valueChanged(int arg1);
    void on_comboBox_currentTextChanged(const QString &arg1);
    //useless
    void debug_only();

private:
    void set_initial_conditions();
    void judge_response(lane::Judge_result);

    void pause(); //in playing mode
    void resume(); //in playing mode
    void parse_tiles(QString file_name);
    void creative_addline(int lane_num);
    void setup_lanekeys(QString);
    void setup_info(QString); //for the tree
    bool input_standard_begin(QString);
    void input_standard_end();
    bool output_standard(QString);
    void disable_tree();
    void label_set_adjust(QLabel *label, QString newtext); //setting up auto-resize text in labels
    //void update_events();

    Ui::mania_window *ui;
    //the only scene in use
    QGraphicsScene thescene{0,0,0,0};

    //everything should stay within this rect, fix aspect ratio here
    QRectF bound_rect{0,0,400,600}; //just a bound to the graphicsview
    const qreal viewtoscene_scale{4}; //1 pixel on the view = 4 pixels in the scene


    //game_mode and stage related
    bool ispaused{false};
    Game_Mode game_mode{Undefined};
    Game_Status game_status{Waiting};

    //local images, debug only
    QGraphicsPixmapItem *localitems[100]; //pointers to temp local items
    QString images[100];
    //local texts for guiding the user
    QGraphicsTextItem *text_item{nullptr}, *warning_item{nullptr};

    //the timers
    QTimer *global_timer{nullptr}, *global_timer2{nullptr};
    int refresh_rate{1};
    qint64 qtime_previous{0}, qtime_now{0}, qtime_elapsed{0}, qtime_paused{0};
    QElapsedTimer *elapsed_timer{nullptr}, *pause_timer{nullptr};

    //score calculation
    int combo{0} ,score{0};

    //now media_player and read/write files
    QMediaPlayer *music_player{nullptr};
    QFile *input{nullptr}, *output{nullptr};
    QTextStream *in_stream{nullptr}, *out_stream{nullptr};

    //the lanes
    int num_lanes{4};
    lane *thelanes[100]; //at most 100 lanes (theorectically)
    QString lane_file_name;
    int lane_keys[100]; //storing the keys, user need to set them every time they open this window though...

    //parsing tiles
    struct Tile_Adding_Info{ //the struct in use
        lane::Add_Catagory catagory{lane::Add_Catagory::Normal};
        int lane_num{0};
        int time_to_add_sec{3}, time_to_add_ms{0};
        double real_time_to_add{0.0};
        int long_cycle{-1};
    };
    QList<QList<Tile_Adding_Info>> tile_info_atlane; //use it like a 2d array+queue

    //song and beatmap paths and the treewidget to display them
    int num_songs{0};
    QList<QTreeWidgetItem*> tree_items;
    QList<QString> song_paths, beatmap_paths;
    QString current_song_path{"/Mario/super_mario.mp3"}, current_beatmap_path{"/mario_tiles.txt"};
    QString info_file_name{"/bundles/songs_and_maps.txt"};

    //Own folder name
    const QString folder_for_all{"c:/orz/AppData"}; //convention: '/' is left-associative

    //recording, not implemented, although might  be similar to creative mode
    bool to_record{false};

    void closeEvent(QCloseEvent *);
};

#endif // MANIA_WINDOW_H
