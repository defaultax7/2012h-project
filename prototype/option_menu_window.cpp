#include "option_menu_window.h"
#include "ui_option_menu_window.h"

#include <QMessageBox>
#include <QTextStream>
#include <QFileInfo>
#include <QSettings>
#include <QDebug>

option_menu_window::option_menu_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::option_menu_window)
{
    ui->setupUi(this);

    //    QString config_file_path = "config.txt";

    //    QFileInfo check_file(config_file_path);
    //    if (check_file.exists() && check_file.isFile()) {
    //        if(validate_config_file()){
    //            ifstream config_file("config.txt");
    //            config_file >> setting.music_vol;
    //            config_file >> setting.effect_vol;
    //            ui->effect_vol_slider->setValue(setting.effect_vol);
    //            ui->music_vol_slider->setValue(setting.music_vol);
    //        }
    //    } else {
    //        QMessageBox msgBox;
    //        msgBox.setText("Config file was not found, a new config file will be created");
    //        msgBox.exec();

    //        // create config file
    //        QFile file;
    //        file.setFileName("config.txt");
    //        if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
    //        {
    //            QMessageBox msgBox;
    //            msgBox.setText("Create fail, please check the permission of the folder");
    //            msgBox.exec();
    //        }else{
    //            QTextStream outStream(&file);
    //            outStream << "100" << "100";
    //        }
    //        // default volume 100
    //        ui->effect_vol_slider->setValue(100);
    //        ui->music_vol_slider->setValue(100);

    //        file.close();
    //    }

}

option_menu_window::~option_menu_window()
{
    delete ui;
}

bool option_menu_window::validate_config_file(){
    // dummy function, implement it if has time left
    return true;
}

void option_menu_window::map_setting_to_ui()
{
    QSettings setting("HKUST" , "ORZ");
    ui->effect_vol_slider->setValue(setting.value("effect_vol").toInt());
    ui->music_vol_slider->setValue(setting.value("music_vol").toInt());
}

void option_menu_window::showEvent(QShowEvent *event)
{
    QSettings setting("HKUST" , "ORZ");

    // Set to 100 if setting is not found
    if(!setting.value("music_vol").isValid()){
        setting.setValue("music_vol",100);
    }
    if(!setting.value("effect_vol").isValid()){
        setting.setValue("effect_vol",100);
    }

    map_setting_to_ui();
}

void option_menu_window::on_pushButton_clicked()
{
    this->close();
}

void option_menu_window::closeEvent(QCloseEvent *){
    parentWidget()->show();
}

void option_menu_window::on_horizontalSlider_valueChanged(int value)
{
    // seem like if the name is changed, this will not work anymore
}

void option_menu_window::on_pushButton_2_clicked()
{
}

void option_menu_window::on_music_vol_slider_valueChanged(int value)
{
    QSettings setting("HKUST" , "ORZ");
    ui->txt_music_vol->setText(QString::number(value));
    setting.setValue("music_vol", value);
}

void option_menu_window::on_effect_vol_slider_valueChanged(int value)
{
    QSettings setting("HKUST" , "ORZ");
    ui->txt_effect_vol->setText(QString::number(value));
    setting.setValue("effect_vol", value);
}
