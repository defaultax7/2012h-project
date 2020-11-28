QT       += core gui
QT += widgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    music_player_window.cpp \
    taiko/normal_note.cpp \
    taiko/note.cpp \
    option_menu_window.cpp \
    score.cpp \
    taiko/taiko_performance_view.cpp \
    taiko/taiko_window.cpp

HEADERS += \
    mainwindow.h \
    music_player_window.h \
    taiko/normal_note.h \
    taiko/note.h \
    option_menu_window.h \
    score.h \
    taiko/taiko_performance_view.h \
    taiko/taiko_window.h

FORMS += \
    mainwindow.ui \
    music_player_window.ui \
    option_menu_window.ui \
    taiko/taiko_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
