#ifndef CLICKABLE_VIEW_H
#define CLICKABLE_VIEW_H

#include <QGraphicsView>

class clickable_view : public QGraphicsView
{
    Q_OBJECT
public:
    clickable_view(QWidget *parent);

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void mouseClicked(int row, int col);
};

#endif // CLICKABLE_VIEW_H
