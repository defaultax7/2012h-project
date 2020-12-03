#ifndef MAP_SELECTION_WINDOW_H
#define MAP_SELECTION_WINDOW_H

#include "Taiko_map.h"

#include <QMainWindow>
#include <QMediaPlayer>
#include <QString>
#include <QTreeWidgetItem>

namespace Ui {
class map_selection_window;
}

class map_selection_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit map_selection_window(QWidget *parent = nullptr);
    ~map_selection_window();
    void showEvent(QShowEvent* event);

private slots:
    void on_btn_auto_mode_clicked();

    void on_btn_high_speed_mode_clicked();

    void on_btn_dark_mode_clicked();

    void on_btn_fade_out_mode_clicked();

    void on_btn_random_mode_clicked();

    void on_btn_back_clicked();

    void on_map_tree_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::map_selection_window *ui;
    QString btn_active_style;
    QString btn_inactive_style;
    QList<Taiko_map*> map_list;
    QMediaPlayer* music_player;
    bool auto_mode = false;
    bool high_speed_mode = false;
    bool dark_mode = false;
    bool fade_out_mode = false;
    bool random_mode = false;

    void closeEvent(QCloseEvent *);
    void add_map(QString map_path);
    bool file_exist(QString path);
};

#endif // MAP_SELECTION_WINDOW_H
