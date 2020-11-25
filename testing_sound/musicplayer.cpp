#include "musicplayer.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>


MusicPlayer::MusicPlayer(QWidget *parent) : QWidget(parent)
{
    QString button_style = "QPushButton { \
            font-size: 40px;\
            font: bold large \"Aerial\";\
            color:rgb(17,17,17);\
            border-width: 1px;\
            border-radius: 6px;\
            border-bottom-color: rgb(150,150,150);\
            border-right-color: rgb(165,165,165);\
            border-left-color: rgb(165,165,165);\
            border-top-color: rgb(180,180,180);\
            border-style: solid;\
            padding: 4px;\
            background-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(255, 255, 255, 255));\
        }\
        \
        QPushButton:hover {\
            border-top-color: rgb(76,195,255);\
            border-right-color: qlineargradient(spread:pad, x1:0, y1:1, x2:1, y2:0, stop:0 rgb(76,195,255), stop:1 rgb(76,195,255));\
            border-left-color:  qlineargradient(spread:pad, x1:1, y1:0, x2:0, y2:0, stop:0 rgb(76,195,255), stop:1 rgb(76,195,255));\
            border-bottom-color: rgb(76,195,255);\
            background-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(255, 255, 255, 255));\
        }\
        QPushButton:pressed{\
            background-color: qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:1, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(255, 255, 255, 255));\
        }";

    QString combobox_style = "QComboBox { \
            font-size: 20px;\
            font: bold large \"Aerial\";\
            color:rgb(17,17,17);\
            border-width: 1px;\
            border-radius: 6px;\
            border-bottom-color: rgb(150,150,150);\
            border-right-color: rgb(165,165,165);\
            border-left-color: rgb(165,165,165);\
            border-top-color: rgb(180,180,180);\
            border-style: solid;\
            padding: 4px;\
            background-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(255, 255, 255, 255));\
        }";

    m_playButton = new QPushButton(this);
    m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    m_playButton->setStyleSheet(button_style);

    m_stopButton = new QPushButton(this);
    m_stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    m_stopButton->setStyleSheet(button_style);
    m_stopButton->setEnabled(false);

    m_nextButton = new QPushButton(this);
    m_nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    m_nextButton->setStyleSheet(button_style);

    m_previousButton = new QPushButton(this);
    m_previousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    m_previousButton->setStyleSheet(button_style);

    m_muteButton = new QPushButton(this);
    m_muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    m_muteButton->setStyleSheet(button_style);

    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);

    m_rateBox = new QComboBox(this);
    m_rateBox->addItem("0.5x", QVariant(0.5));
    m_rateBox->addItem("1.0x", QVariant(1.0));
    m_rateBox->addItem("2.0x", QVariant(2.0));
    m_rateBox->setCurrentIndex(1);
    m_rateBox->setStyleSheet(combobox_style);

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
