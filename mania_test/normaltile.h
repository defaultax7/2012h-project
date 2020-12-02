#ifndef NORMALTILE_H
#define NORMALTILE_H

#include "newtile.h"

class NormalTile : public NewTile
{
public:
    NormalTile(QPointF start_point, QPointF end_point, QPointF delta, QObject *parent=nullptr);
    virtual void init(TileType, int);
    virtual bool update_remove();
};

#endif // NORMALTILE_H
