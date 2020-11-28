#ifndef NOTE_H
#define NOTE_H

#include <QGraphicsPixmapItem>
#include <QTimer>

class Note : public QObject
{
    Q_OBJECT
public:
    Note(QObject* parent= nullptr);
    ~Note();
//    virtual void get_hit() = 0;
    void start_move();
    void init(QGraphicsPixmapItem* , double , double);
    void set_image_item(QGraphicsPixmapItem*);

private:
    double speed = 1;  // should scroll to left only
    double row = 0;
    double col = 0;
    QGraphicsPixmapItem* image_item;
    QTimer* timer;
private slots:
    void move();
};

#endif // NOTE_H
