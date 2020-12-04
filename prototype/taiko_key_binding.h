#ifndef TAIKO_KEY_BINDING_H
#define TAIKO_KEY_BINDING_H

#include <QMainWindow>
#include <QKeyEvent>

namespace Ui {
class taiko_key_binding;
}

class taiko_key_binding : public QMainWindow
{
    Q_OBJECT

public:
    enum key_setting{ drum_l , drum_r , rim_l , rim_r };

    explicit taiko_key_binding(QWidget *parent = nullptr);
    ~taiko_key_binding();

    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_btn_back_clicked();

    void on_btn_add_drum_l_clicked();

    void on_btn_remove_drum_l_clicked();

    void on_btn_add_drum_r_clicked();

    void on_btn_remove_drum_r_clicked();

    void on_btn_add_rim_l_clicked();

    void on_btn_remove_rim_l_clicked();

    void on_btn_add_rim_r_clicked();

    void on_btn_remove_rim_r_clicked();

private:
    Ui::taiko_key_binding *ui;
    bool setting_key = false;
    key_setting current_key_setting;

    void showEvent(QShowEvent*);

    void show_smoke_screen();
    void hide_smoke_screen();

};

#endif // TAIKO_KEY_BINDING_H
