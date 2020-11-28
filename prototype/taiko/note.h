#ifndef NOTE_H
#define NOTE_H

#include <QGraphicsPixmapItem>
#include <QTimer>

class Note : public QObject
{
    Q_OBJECT
public:
    Note(double x, double y, double endpoint, double speed , QObject* parent= nullptr);
    ~Note();
//    virtual void get_hit() = 0;
    void start_move();
    virtual void init(QGraphicsScene&) = 0;

protected:
    QGraphicsPixmapItem* image_item;
    double x;
    double y;

private:
    const unsigned int refresh_rate = 1;  // the unit is ms & is not unexpected to be changed
    const double speed = 1;  // should scroll to left only
    const double offset = 0;
    const double endpoint = 0; // remove the note when reach the endpoint

    QTimer* timer;

private slots:
    void move();
    virtual void get_hit() = 0;
};

#endif // NOTE_H
