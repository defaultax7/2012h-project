#include <fstream>
using std::ifstream;
using std::noskipws;
using std::ws;
#include <iostream>
using std::cout;
using std::endl;

#include <QDebug>
#include "mania_window.h"
#include "ui_mania_window.h"
#include "clickable_view.h"

mania_window::mania_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mania_window)
{
    //now i got a status bar, a menu, a black background...
    ui->setupUi(this);
    ui->gview->setScene(&thescene);
    ui->gview->show();

    //bound_rect_item = thescene.addRect(bound_rect); //this is the bounds
    //QRectF bounds = thescene.itemsBoundingRect();
    //ui->gview->fitInView(bounds, Qt::KeepAspectRatio);
    QRectF weird = thescene.sceneRect();
    qDebug() << weird.width() << " " << weird.height() << "weird" << endl;

    //ui->gview->fitInView(bound_rect, Qt::KeepAspectRatio); //dont use it...
    ui->gview->setAlignment(Qt::AlignTop|Qt::AlignLeft);

    //ui->menuStart->setVisible(false); //why i can still see them???
    //ui->menuUseless->setVisible(false); //these 2 are obdolete menu bar
    ui->actionaction_2->setVisible(false); //this is new toolbar version

    //set images
    images[0] = ":/image/mania_test/images/small_square.png";
    images[1] = ":/image/mania_test/images/minions.jpg";

    //try to add them
    localitems[0] = thescene.addPixmap(QPixmap(images[0]));
    localitems[0]->setOffset(100,100);

    //localitems[1] = thescene.addPixmap(QPixmap(images[1]));
    //localitems[1]->setOffset(200,200);

    weird = thescene.sceneRect();
    qDebug() << weird.width() << " " << weird.height() << "weird" << endl;

    //ui->gview->resize(500,300); it is resizing the view itself...
    ui->gview->scale(0.25,0.25); //worked, but not good though?
    bound_rect.setWidth(bound_rect.width()/0.25);
    bound_rect.setHeight(bound_rect.height()/0.25);
    qDebug() << bound_rect.width() << " " << bound_rect.height() << "bound_weird" << endl;

    //connect clickable view to some function
    connect(ui->gview, &clickable_view::mouseClicked, this, &mania_window::ui_view_clicked);

    //disable second button
    ui->pushButton_2->setEnabled(false);

    //first i need to initialize the lanes
    thelanes[0] = new lane(Qt::Key_D);
    thelanes[1] = new lane(Qt::Key_E);
    thelanes[2] = new lane(Qt::Key_S);
    thelanes[3] = new lane(Qt::Key_P);

    //now add the lanes to the scene
    for (int i=0; i<num_lanes; ++i){
        if (thelanes[i] == nullptr){
            qDebug() << "you foolish null pointer: lane " << i << endl;
            continue;
        }
        thelanes[i]->add_to_scene(i, num_lanes, &thescene, bound_rect); //scene can only pass by pointer/reference?
    }

    //try to connect dying tiles to a function
    //not here...
}

mania_window::~mania_window()
{
    delete ui;
}

void mania_window::ui_view_clicked(int xpos, int ypos){
    qDebug() << "view coordinate: " <<xpos << " " << ypos << endl;
    QPointF scene_coor = ui->gview->mapToScene(xpos, ypos); //scenecoordinate...
    qDebug() << "scene coordinate: " << scene_coor.x() << " " << scene_coor.y() << endl;

}

void mania_window::on_pushButton_clicked()
{
    //now add a timer to it
    global_timer = new QTimer;
    connect(global_timer, &QTimer::timeout, this, &mania_window::update);
    global_timer->start(refresh_rate);
    //set the buttons
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);

}
void mania_window::on_pushButton_2_clicked()
{
    ui->pushButton_2->setEnabled(false);
}

void mania_window::keyPressEvent(QKeyEvent *event){
    //qDebug() << "keypressed" << endl;
    //(event->isAutoRepeat)
    for (int i=0; i<num_lanes; ++i){
        if (event->key() == thelanes[i]->getkeynum() ){
            if (!event->isAutoRepeat()){
                qDebug() << "lane " << i << "is pressed." << endl;
                judge_response(thelanes[i]->on_key_pressed(&thescene));
            }
            return;
        }
    }
}

void mania_window::keyReleaseEvent(QKeyEvent *event){
    if (!event->isAutoRepeat()){
        for (int i=0; i<num_lanes; ++i){
            if (event->key() == thelanes[i]->getkeynum() ){
                qDebug() << "lane " << i << "is released." << endl;
                thelanes[i]->on_key_released(&thescene);
                return;
            }
        }
    }
}

void mania_window::update(){
    timeelasped = (timeelasped + refresh_rate) % 2000;
    for (int i=0; i<num_lanes; ++i){
        if (timeelasped == i*200+400) {
            NewTile* added_tile = thelanes[i]->addtile(lane::Tile_Catagory::Normal, &thescene);
            if (added_tile != nullptr) connect(added_tile, &NewTile::die, this, &mania_window::missed_tile_response);
        }
        thelanes[i]->update(&thescene);
    }
}

void mania_window::judge_response(lane::Judge_result result){
    if (result == lane::Judge_result::Good){
        combo++;
        score+= (0.1*combo + 1)*300;
    }else{
        combo=0;
        score-= 200;
    }

    ui->lcd_combo->display(combo);
    ui->lcd_score->display(score);
}

void mania_window::missed_tile_response(){
    combo=0;
    score-= 200;
    ui->lcd_combo->display(combo);
    ui->lcd_score->display(score);
}
