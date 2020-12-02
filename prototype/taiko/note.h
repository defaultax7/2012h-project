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
    virtual void get_hit(int performance) = 0;
    void start_move();
    virtual void init(QGraphicsScene*) = 0;
    int getX() const;
    void stop();
    void unstop();

protected:
    QGraphicsPixmapItem* image_item = nullptr;
    double x;
    double y;
    double speed = 1;  // should scroll to left only
    QTimer* timer;
    const unsigned int afterlife_time = 200;  // unit ms

signals:
    void die();
    void note_was_missed();
    void note_was_hit(int);

private:
    const unsigned int refresh_rate = 1;  // the unit is ms & is not unexpected to be changed
    const double offset = 0;
    const double endpoint = 0; // remove the note when reach the endpoint
    bool is_stop = false;

private slots:
    void move();
//    virtual void get_hit() = 0;
};

#endif // NOTE_H
