#ifndef MPWINDOW_H
#define MPWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>

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

    void on_btn_open_music_clicked();

    void on_search_song_textChanged(const QString &arg1);

private:
    Ui::mpwindow *ui;
    QMediaPlayer *m_player = nullptr;
    bool is_mute = false;

    void closeEvent(QCloseEvent *);
};

#endif // MPWINDOW_H
