#include "result_window.h"
#include "ui_result_window.h"

#include <taiko/map/map_selection_window.h>

#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QGraphicsPixmapItem>
#include <QDirIterator>

result_window::result_window(Performance performance, QString map_path, QWidget *parent) :
    QMainWindow(parent), performance(performance),
    ui(new Ui::result_window)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(&scene);

    QFileInfo temp(map_path);

    // append the result to result file ( file will be created if not existed)
    QFile file(temp.dir().path() + "/result.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream out(&file);
        out << performance.score.get_score();
        out << " ";
        QDateTime datetime;
        out << datetime.currentDateTime().toString("dd.MM.yyyy-hh:mm:ss") + "\n";
    }
}


result_window::~result_window()
{
    delete ui;
}

void result_window::closeEvent(QCloseEvent *){
    map_selection_window* w = new map_selection_window();
    w->show();
    close();
}

void result_window::showEvent(QShowEvent *)
{
    // set the background image
    scene.addPixmap(QPixmap(":/image/image/Background.png"));
    ui->graphicsView->fitInView(scene.sceneRect(), Qt::KeepAspectRatio);

    // add rank image
    QString rank;
    if(performance.get_rank() == Performance::S){
        rank = "s";
    }else if(performance.get_rank() == Performance::A){
        rank = "a";
    }else if(performance.get_rank() == Performance::B){
        rank = "b";
    }else if(performance.get_rank() == Performance::C){
        rank = "c";
    }else if(performance.get_rank() == Performance::D){
        rank = "d";
    }
    scene.addPixmap(QPixmap(":/image/image/" + rank + ".png"));

    show_number_with_image(performance.score.get_score() , 740 , 80);

    int vertical_gap = 70;
    int y = 150;
    int x = 500;
    QGraphicsPixmapItem* perfect = scene.addPixmap(QPixmap(":/image/image/result_perfect.png"));
    QGraphicsPixmapItem* good = scene.addPixmap(QPixmap(":/image/image/result_good.png"));
    QGraphicsPixmapItem* miss = scene.addPixmap(QPixmap(":/image/image/result_miss.png"));
    QGraphicsPixmapItem* combo = scene.addPixmap(QPixmap(":/image/image/combo.png"));
    perfect->setPos(x,y);
    good->setPos(x,y + vertical_gap);
    miss->setPos(x,y + vertical_gap * 2);
    combo->setPos(x,y + vertical_gap * 3 + 15);

    int horizontal_gap_between_perfect = 240;
    show_number_with_image(performance.perfect , x + horizontal_gap_between_perfect , y + 10);
    show_number_with_image(performance.good , x + horizontal_gap_between_perfect , y + 10 + vertical_gap);
    show_number_with_image(performance.miss , x + horizontal_gap_between_perfect , y + 10 + vertical_gap * 2);
    show_number_with_image(performance.highest_combo , x + horizontal_gap_between_perfect , y + 10 + vertical_gap * 3);

}

void result_window::show_number_with_image(int number, int x, int y)
{
    // show the digit with image from right to left
    while(number >= 10){
        int num = number % 10;
        QGraphicsPixmapItem* temp = scene.addPixmap(QPixmap(":/image/image/" + QString::number(num) + ".png"));
        temp->setPos(x , y);
        x -= digit_gap;
        number = number / 10;
    }
    // show the last digit
    QGraphicsPixmapItem* temp = scene.addPixmap(QPixmap(":/image/image/" + QString::number(number) + ".png"));
    temp->setPos(x , y);
}

void result_window::on_btn_back_clicked()
{
    close();
}
