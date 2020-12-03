QT       += core gui
QT += widgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mania_test/clickable_view.cpp \
    mania_test/lane.cpp \
    mania_test/longtile.cpp \
    mania_test/mania_window.cpp \
    mania_test/newtile.cpp \
    mania_test/normaltile.cpp \

HEADERS += \
    mania_test/clickable_view.h \
    mania_test/lane.h \
    mania_test/longtile.h \
    mania_test/mania_window.h \
    mania_test/newtile.h \
    mania_test/normaltile.h \

FORMS += \
    mania_test/mania_window.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
