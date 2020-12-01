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

    //ui->gview->fitInView(bound_rect, Qt::KeepAspectRatio);
    //dont use it...
    ui->gview->setAlignment(Qt::AlignTop|Qt::AlignLeft);

    //ui->menuStart->setVisible(false); //why i can still see them???
    //ui->menuUseless->setVisible(false); //these 2 are menu bar
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
    //now add the lanes to the scene
    for (int i=0; i<num_lanes; ++i){
        thelanes[i].add_to_scene(i, num_lanes, &thescene, bound_rect); //scene can only pass by pointer/reference?
    }
    ui->pushButton->setEnabled(false);

    lanekeys[0] = Qt::Key_D;
    lanekeys[1] = Qt::Key_E;
    lanekeys[2] = Qt::Key_F;
    lanekeys[3] = Qt::Key_G;
}
void mania_window::keyPressEvent(QKeyEvent *event){
    //qDebug() << "keypressed" << endl;
    for (int i=0; i<num_lanes; ++i){
        if (event->key() == lanekeys[i] ){
            qDebug() << "lane " << i << "is pressed." << endl;
            thelanes[i].on_key_pressed(&thescene);
            return;
        }
    }
}
