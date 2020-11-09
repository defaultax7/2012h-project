#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QAbstractButton>
#include <QAbstractSlider>

class QComboBox;
class MusicPlayer : public QWidget
{
    Q_OBJECT
public:
    explicit MusicPlayer(QWidget *parent = nullptr);

signals:

private:
    QMediaPlayer::State m_playerState = QMediaPlayer::StoppedState;
    bool m_playerMuted = false;
    QAbstractButton *m_playButton = nullptr;
    QAbstractButton *m_stopButton = nullptr;
    QAbstractButton *m_nextButton = nullptr;
    QAbstractButton *m_previousButton = nullptr;
    QAbstractButton *m_muteButton = nullptr;
    QAbstractSlider *m_volumeSlider = nullptr;
    QComboBox *m_rateBox = nullptr;
};

#endif // MUSICPLAYER_H
