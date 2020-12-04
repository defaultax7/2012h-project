#include "mainwindow.h"
#include "option_menu_window.h"
#include "taiko_key_binding.h"
#include "ui_option_menu_window.h"

#include <QMessageBox>
#include <QTextStream>
#include <QFileInfo>
#include <QSettings>
#include <QDebug>
#include <QIntValidator>

option_menu_window::option_menu_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::option_menu_window)
{
    ui->setupUi(this);

    // set the input range for offset ( more than 2 second is not accepatable)
    ui->txt_offset->setValidator( new QIntValidator(0, 999, this) );

}

option_menu_window::~option_menu_window()
{
    delete ui;
}

void option_menu_window::map_setting_to_ui()
{

    ui->effect_vol_slider->setValue(setting.value("effect_vol").toInt());
    ui->music_vol_slider->setValue(setting.value("music_vol").toInt());

    // when setting vol = 100, on change event will not be trigger, set it manually
    ui->txt_music_vol->setText(setting.value("music_vol").toString());
    ui->txt_effect_vol->setText(setting.value("effect_vol").toString());
    ui->txt_offset->setText(setting.value("offset").toString());
}

void option_menu_window::showEvent(QShowEvent *event)
{
    map_setting_to_ui();
}

void option_menu_window::on_pushButton_clicked()
{
    this->close();
}

void option_menu_window::closeEvent(QCloseEvent *){
    MainWindow* w = new MainWindow();
    w->show();
    close();
}

void option_menu_window::on_pushButton_2_clicked()
{
    taiko_key_binding* w = new taiko_key_binding();
    w->show();
}

void option_menu_window::on_music_vol_slider_valueChanged(int value)
{
    ui->txt_music_vol->setText(QString::number(value));
    setting.setValue("music_vol", value);
}

void option_menu_window::on_effect_vol_slider_valueChanged(int value)
{
    ui->txt_effect_vol->setText(QString::number(value));
    setting.setValue("effect_vol", value);
}

void option_menu_window::on_txt_offset_textChanged(const QString &arg1)
{
    setting.setValue("offset", arg1.toInt());
}
