#include "spokewire.h"

SpokeWire::SpokeWire(const qreal &pos_y, QObject *parent) :
    QObject{parent},
    QGraphicsItem{},
    POS_Y{pos_y}
{}

SpokeWire::~SpokeWire()
{}

QRectF SpokeWire::boundingRect() const
{
    return QRectF(0, 0, 590, 2);
}

void SpokeWire::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    this->setPos(0, POS_Y);
    painter->setBrush(Qt::gray);
    painter->drawRect(0, 0, 590, 2);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}
