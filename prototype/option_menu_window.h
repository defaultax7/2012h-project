#ifndef OPTION_MENU_WINDOW_H
#define OPTION_MENU_WINDOW_H

#include <QMainWindow>

namespace Ui {
class option_menu_window;
}

class option_menu_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit option_menu_window(QWidget *parent = nullptr);
    ~option_menu_window();

private slots:
    void on_pushButton_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_2_clicked();

    void on_music_vol_slider_valueChanged(int value);

    void on_effect_vol_slider_valueChanged(int value);

private:
    Ui::option_menu_window *ui;

    void closeEvent(QCloseEvent *);
    bool validate_config_file();
    void map_setting_to_ui();
};

#endif // OPTION_MENU_WINDOW_H
