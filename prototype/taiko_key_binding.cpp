#include "taiko_key_binding.h"
#include "ui_taiko_key_binding.h"
#include "QDebug"

#include <QMessageBox>
#include <QSettings>

taiko_key_binding::taiko_key_binding(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::taiko_key_binding)
{
    ui->setupUi(this);

    // retrieve key setting
    QSettings setting("HKUST" , "ORZ");

    // map the key setting to the list view
    if(setting.contains("drum_l_key")){
        drum_l_list_string = setting.value("drum_l_key").toStringList();
    }
    if(setting.contains("drum_r_key")){
        drum_r_list_string = setting.value("drum_r_key").toStringList();
    }
    if(setting.contains("rim_l_key")){
        rim_l_list_string = setting.value("rim_l_key").toStringList();
    }
    if(setting.contains("rim_r_key")){
        rim_r_list_string = setting.value("rim_r_key").toStringList();
    }

}

taiko_key_binding::~taiko_key_binding()
{
    delete ui;
}

void taiko_key_binding::keyPressEvent(QKeyEvent *event)
{
    if(setting_key){

        QSettings setting("HKUST" , "ORZ");

        // Cancel
        if(event->key() == Qt::Key_Escape){
            hide_smoke_screen();
            setting_key = false;
        }else if(event->key() == Qt::Key_Space){
            // space is reserved
            QMessageBox msgBox;
            msgBox.setWindowTitle("Warning");
            msgBox.setText("Space is reserved!");
            msgBox.exec();
        }else{
            QString key_to_String = QKeySequence(event->key()).toString();
            if(drum_l_list_string.contains(key_to_String) || drum_r_list_string.contains(key_to_String) || rim_r_list_string.contains(key_to_String) || rim_l_list_string.contains(key_to_String)){
                QMessageBox msgBox;
                msgBox.setWindowTitle("Warning");
                msgBox.setText("Key was occupied!");
                msgBox.exec();
            }else{
                // add key
                if(current_key_setting == drum_l){
                    drum_l_list_string.append(key_to_String);
                    map_list_to_list_view(drum_l_list_string , ui->drum_l_list);
                    setting.setValue("drum_l_key" , drum_l_list_string);
                }else if(current_key_setting == drum_r){
                    drum_r_list_string.append(key_to_String);
                    map_list_to_list_view(drum_r_list_string , ui->drum_r_list);
                    setting.setValue("drum_r_key" , drum_r_list_string);
                }else if(current_key_setting == rim_l){
                    rim_l_list_string.append(key_to_String);
                    map_list_to_list_view(rim_l_list_string , ui->rim_l_list);
                    setting.setValue("rim_l_key" , rim_l_list_string);
                }else if(current_key_setting == rim_r){
                    rim_r_list_string.append(key_to_String);
                    map_list_to_list_view(rim_r_list_string , ui->rim_r_list);
                    setting.setValue("rim_r_key" , rim_r_list_string);
                }
                hide_smoke_screen();
                setting_key = false;
            }
        }
    }
}

void taiko_key_binding::on_btn_back_clicked()
{
    close();
}

void taiko_key_binding::showEvent(QShowEvent *)
{
    hide_smoke_screen();

    // map the key setting to the list view
    map_list_to_list_view(drum_l_list_string , ui->drum_l_list);
    map_list_to_list_view(drum_r_list_string , ui->drum_r_list);
    map_list_to_list_view(rim_l_list_string , ui->rim_l_list);
    map_list_to_list_view(rim_r_list_string , ui->rim_r_list);

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

void taiko_key_binding::map_list_to_list_view(QStringList key_list, QListWidget* list_view)
{
    list_view->clear();
    for(int i = 0 ; i < key_list.count() ; ++i){
        list_view->addItem(key_list.at(i));
    }
}

void taiko_key_binding::on_btn_add_drum_l_clicked()
{
    show_smoke_screen();
    setting_key = true;
    current_key_setting = drum_l;
}

void taiko_key_binding::on_btn_remove_drum_l_clicked()
{
    // if key is selected , remove it from setting
    if(!ui->drum_l_list->selectedItems().empty()){
        QSettings setting("HKUST" , "ORZ");
        drum_l_list_string.removeAll(ui->drum_l_list->selectedItems().at(0)->data(0).toString());
        map_list_to_list_view(drum_l_list_string , ui->drum_l_list);
        setting.setValue("drum_l_key" , drum_l_list_string);
    }
}

void taiko_key_binding::on_btn_add_drum_r_clicked()
{
    show_smoke_screen();
    setting_key = true;
    current_key_setting = drum_r;
}

void taiko_key_binding::on_btn_remove_drum_r_clicked()
{
    // if key is selected , remove it from setting
    if(!ui->drum_r_list->selectedItems().empty()){
        QSettings setting("HKUST" , "ORZ");
        drum_r_list_string.removeAll(ui->drum_r_list->selectedItems().at(0)->data(0).toString());
        map_list_to_list_view(drum_r_list_string , ui->drum_r_list);
        setting.setValue("drum_r_list" , drum_r_list_string);
    }
}

void taiko_key_binding::on_btn_add_rim_l_clicked()
{
    show_smoke_screen();
    setting_key = true;
    current_key_setting = rim_l;
}

void taiko_key_binding::on_btn_remove_rim_l_clicked()
{
    // if key is selected , remove it from setting
    if(!ui->rim_l_list->selectedItems().empty()){
        QSettings setting("HKUST" , "ORZ");
        rim_l_list_string.removeAll(ui->rim_l_list->selectedItems().at(0)->data(0).toString());
        map_list_to_list_view(rim_l_list_string , ui->rim_l_list);
        setting.setValue("rim_l_list" , rim_l_list_string);
    }
}

void taiko_key_binding::on_btn_add_rim_r_clicked()
{
    show_smoke_screen();
    setting_key = true;
    current_key_setting = rim_r;
}

void taiko_key_binding::on_btn_remove_rim_r_clicked()
{
    // if key is selected , remove it from setting
    if(!ui->rim_r_list->selectedItems().empty()){
        QSettings setting("HKUST" , "ORZ");
        rim_r_list_string.removeAll(ui->rim_r_list->selectedItems().at(0)->data(0).toString());
        map_list_to_list_view(rim_r_list_string , ui->rim_r_list);
        setting.setValue("rim_r_list" , rim_r_list_string);
    }
}
