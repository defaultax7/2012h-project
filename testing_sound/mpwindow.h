#ifndef MPWINDOW_H
#define MPWINDOW_H

#include <QMainWindow>

namespace Ui {
class mpwindow;
}

class mpwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit mpwindow(QWidget *parent = nullptr);
    ~mpwindow();

private slots:
    void on_btn_mute_clicked();

private:
    Ui::mpwindow *ui;
    bool is_mute = false;

    void closeEvent(QCloseEvent *);
};

#endif // MPWINDOW_H
