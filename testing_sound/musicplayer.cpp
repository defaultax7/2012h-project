#include "musicplayer.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QToolButton>

MusicPlayer::MusicPlayer(QWidget *parent) : QWidget(parent)
{
    m_playButton = new QToolButton(this);
    m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    m_stopButton = new QToolButton(this);
    m_stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    m_stopButton->setEnabled(false);

    m_nextButton = new QToolButton(this);
    m_nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));

    m_previousButton = new QToolButton(this);
    m_previousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));

    m_muteButton = new QToolButton(this);
    m_muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);

    m_rateBox = new QComboBox(this);
    m_rateBox->addItem("0.5x", QVariant(0.5));
    m_rateBox->addItem("1.0x", QVariant(1.0));
    m_rateBox->addItem("2.0x", QVariant(2.0));
    m_rateBox->setCurrentIndex(1);

    QBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_stopButton);
    layout->addWidget(m_previousButton);
    layout->addWidget(m_playButton);
    layout->addWidget(m_nextButton);
    layout->addWidget(m_muteButton);
    layout->addWidget(m_volumeSlider);
    layout->addWidget(m_rateBox);
    setLayout(layout);
}
