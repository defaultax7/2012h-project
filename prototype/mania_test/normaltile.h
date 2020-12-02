#ifndef NORMALTILE_H
#define NORMALTILE_H

#include "newtile.h"

class NormalTile : public NewTile
{
public:
    enum TileType{White, Pink, Mario, Undefined};
    NormalTile(QPointF start_point, QPointF end_point, QPointF delta,TileType, QObject *parent=nullptr);
    virtual void init(int);
    virtual bool update_remove();
private:
    TileType tile_type{TileType::Undefined};
};

#endif // NORMALTILE_H
