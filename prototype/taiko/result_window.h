#ifndef RESULT_WINDOW_H
#define RESULT_WINDOW_H

#include "performance.h"

#include <QGraphicsScene>
#include <QMainWindow>

namespace Ui {
class result_window;
}

class result_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit result_window(Performance performance , QString map_path ,  QWidget *parent = nullptr);
    ~result_window();

private slots:
    void on_btn_back_clicked();

private:
    QGraphicsScene scene;
    Ui::result_window *ui;
    Performance performance;
    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent *);
    void show_number_with_image(int number, int x , int y);

    const int digit_gap = 30;
};

#endif // RESULT_WINDOW_H
