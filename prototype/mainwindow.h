#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showEvent(QShowEvent * event);

private slots:
    void on_exit_btn_clicked();

    void on_music_btn_clicked();

    void on_option_btn_clicked();

    void on_btn_start_taiko_clicked();

    void on_btn_start_mania_clicked();

private:
    Ui::MainWindow *ui;

    void closeEvent(QCloseEvent*);

};
#endif // MAINWINDOW_H
