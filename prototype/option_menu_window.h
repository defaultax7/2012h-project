#ifndef OPTION_MENU_WINDOW_H
#define OPTION_MENU_WINDOW_H

#include <QMainWindow>
#include <QSettings>

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

    void on_pushButton_2_clicked();

    void on_music_vol_slider_valueChanged(int value);

    void on_effect_vol_slider_valueChanged(int value);

    void on_txt_offset_textChanged(const QString &arg1);

private:
    Ui::option_menu_window *ui;

    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent* event);
    void map_setting_to_ui();

    QSettings setting{"HKUST" , "ORZ"};
};

#endif // OPTION_MENU_WINDOW_H
