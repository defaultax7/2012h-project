#include <QMouseEvent>
#include "clickable_view.h"

clickable_view::clickable_view(QWidget *parent):QGraphicsView(parent) {}

void clickable_view::mousePressEvent(QMouseEvent *event) {
    // TODO: take the x, y position, convert to grid index and send the signal mouseClicked to GameWindow
    // NOTE the coordinate system:
    // +----------> x
    // |
    // |
    // |
    // v
    // y
    // Size of the image is 16x16
    // useful docs: https://doc.qt.io/qt-5/qmouseevent.html
    // convert window coordinate to scene coordinate: https://doc.qt.io/qt-5/qgraphicsview.html#mapToScene
    // send signal: emit mouseClicked(int, int);

    int xpos = event->x();
    int ypos = event->y();
    emit mouseClicked(xpos, ypos);
    //qDebug() << xpos << " " << ypos << endl; //incomplete type here
}
