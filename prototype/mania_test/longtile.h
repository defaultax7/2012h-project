#ifndef LONGTILE_H
#define LONGTILE_H

#include "newtile.h"

class LongTile : public NewTile
{
public:
    enum TileType{White, Pink, Mario, Undefined};
    LongTile(QPointF start_point, QPointF end_point, QPointF delta,TileType, int length=1000, QObject *parent=nullptr);
    virtual void init(int);
    virtual bool update_remove();
    void set_ispresed(bool);
    bool get_ispressed();
    const QPointF get_tail_point();
private:
    TileType tile_type{TileType::Undefined};
    QPointF tail_point;
    int width, length;
    bool ispressed{false}, missed{false};
};

#endif // LONGTILE_H
