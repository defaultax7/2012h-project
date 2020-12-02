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

namespace Ui {
class mania_window;
}

class mania_window : public QMainWindow
{
    Q_OBJECT

public:
    enum Game_Mode{Undefied, Play, Creative};
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

    void close_output();
    void stop_timer();

private:
    void judge_response(lane::Judge_result);
    void pause(); //not implemented now...
    void parse_tiles(QString file_name);
    void creative_addline(int lane_num);
    Ui::mania_window *ui;
    QGraphicsScene thescene{0,0,0,0};
    QGraphicsPixmapItem *localitems[100]; //points to temp local items
    //they should be unders ojects anyway
    QRectF bound_rect{0,0,400,600};
    QGraphicsRectItem *bound_rect_item; //a pointer to bounding rect, but should beuseless
    //everything should stay within this rect, aspect ratio should be 600:400 anyway

    bool ispaused{false};
    Game_Mode game_mode{Undefied};

    //QString images[100]; //useless

    int num_lanes{4};
    lane *thelanes[100];

    //int lanekeys[100];
    QTimer *global_timer;
    int refresh_rate{2}, timeelasped_ms{0}, timeelaseped_sec{0};

    //score calculation
    int combo{0} ,score{0};

    //now media_player and read/write files
    QMediaPlayer *music_player;
    QMediaPlaylist *play_list;
    QFile *input, *output;
    QTextStream *in_stream, *out_stream;

    struct Tile_Adding_Info{
        lane::Add_Catagory catagory;
        int lane_num;
        int time_to_add_sec, time_to_add_ms;
        int long_cycle{-1};
    };
    QList<QList<Tile_Adding_Info>> tile_info_atlane; //this would be initialized by parser in mania_window...
};

#endif // MANIA_WINDOW_H
