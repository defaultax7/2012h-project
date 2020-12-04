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
    music_player/music_player_window.cpp \
    music_player/my_player.cpp \
    taiko/map/Taiko_map.cpp \
    taiko/map/map_selection_window.cpp \
    taiko/normal_note.cpp \
    taiko/note.cpp \
    option_menu_window.cpp \
    score.cpp \
    taiko/note_controller.cpp \
    taiko/performance.cpp \
    taiko/result_window.cpp \
    taiko/taiko_performance_view.cpp \
    taiko/taiko_window.cpp \
    taiko_key_binding.cpp \
    mania/mania_window.cpp \
    mania/clickable_view.cpp \
    mania/lane.cpp \
    mania/longtile.cpp \
    mania/newtile.cpp \
    mania/normaltile.cpp

HEADERS += \
    mainwindow.h \
    music_player/music_player_window.h \
    music_player/my_player.h \
    taiko/map/Taiko_map.h \
    taiko/map/map_selection_window.h \
    taiko/normal_note.h \
    taiko/note.h \
    option_menu_window.h \
    score.h \
    taiko/note_controller.h \
    taiko/performance.h \
    taiko/result_window.h \
    taiko/taiko_performance_view.h \
    taiko/taiko_window.h \
    taiko_key_binding.h \
    mania/mania_window.h \
    mania/clickable_view.h \
    mania/lane.h \
    mania/longtile.h \
    mania/newtile.h \
    mania/normaltile.h

FORMS += \
    mainwindow.ui \
    music_player_window.ui \
    option_menu_window.ui \
    taiko/map_selection_window.ui \
    taiko/result_window.ui \
    taiko/taiko_window.ui \
    taiko_key_binding.ui \
    mania_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
