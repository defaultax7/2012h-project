#include "taiko_key_binding.h"
#include "ui_taiko_key_binding.h"
#include "QDebug"

taiko_key_binding::taiko_key_binding(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::taiko_key_binding)
{
    ui->setupUi(this);
}

taiko_key_binding::~taiko_key_binding()
{
    delete ui;
}

void taiko_key_binding::keyPressEvent(QKeyEvent *event)
{
    // Cancel
    if(event->key() == Qt::Key_Escape){
        hide_smoke_screen();
        setting_key = false;
    }
}

void taiko_key_binding::on_btn_back_clicked()
{
    close();
}

void taiko_key_binding::showEvent(QShowEvent *)
{
    hide_smoke_screen();
}

void taiko_key_binding::show_smoke_screen()
{
    ui->smoke_screen->setVisible(true);
    ui->lb_ask_key->setVisible(true);
}

void taiko_key_binding::hide_smoke_screen()
{
    ui->smoke_screen->setVisible(false);
    ui->lb_ask_key->setVisible(false);
}

void taiko_key_binding::on_btn_add_drum_l_clicked()
{
    show_smoke_screen();
    setting_key = true;
    current_key_setting = drum_l;
}

void taiko_key_binding::on_btn_remove_drum_l_clicked()
{

}

void taiko_key_binding::on_btn_add_drum_r_clicked()
{
    show_smoke_screen();
    setting_key = true;
    current_key_setting = drum_r;
}

void taiko_key_binding::on_btn_remove_drum_r_clicked()
{

}

void taiko_key_binding::on_btn_add_rim_l_clicked()
{
    show_smoke_screen();
    setting_key = true;
    current_key_setting = rim_l;
}

void taiko_key_binding::on_btn_remove_rim_l_clicked()
{

}

void taiko_key_binding::on_btn_add_rim_r_clicked()
{
    show_smoke_screen();
    setting_key = true;
    current_key_setting = rim_r;
}

void taiko_key_binding::on_btn_remove_rim_r_clicked()
{

}
