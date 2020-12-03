#include "result_window.h"
#include "ui_result_window.h"

#include <taiko/map/map_selection_window.h>

result_window::result_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::result_window)
{
    ui->setupUi(this);
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
