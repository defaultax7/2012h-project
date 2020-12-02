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
    mania_test/clickable_view.cpp \
    mania_test/lane.cpp \
    mania_test/mania_window.cpp \
    mania_test/newtile.cpp \
    mania_test/normaltile.cpp \
    music_player/music_player_window.cpp \
    music_player/my_player.cpp \
    taiko/normal_note.cpp \
    taiko/note.cpp \
    option_menu_window.cpp \
    score.cpp \
    taiko/note_controller.cpp \
    taiko/taiko_performance_view.cpp \
    taiko/taiko_window.cpp

HEADERS += \
    mainwindow.h \
    mania_test/clickable_view.h \
    mania_test/lane.h \
    mania_test/mania_window.h \
    mania_test/newtile.h \
    mania_test/normaltile.h \
    music_player/music_player_window.h \
    music_player/my_player.h \
    taiko/normal_note.h \
    taiko/note.h \
    option_menu_window.h \
    score.h \
    taiko/note_controller.h \
    taiko/taiko_performance_view.h \
    taiko/taiko_window.h

FORMS += \
    mainwindow.ui \
    mania_test/mania_window.ui \
    music_player_window.ui \
    option_menu_window.ui \
    taiko/taiko_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
