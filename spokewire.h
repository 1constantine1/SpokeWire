#ifndef SPOKEWIRE_H
#define SPOKEWIRE_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>

class SpokeWire : public QObject, public QGraphicsItem
{

    Q_OBJECT

public:

    explicit SpokeWire(const qreal &pos_y, QObject *parent = nullptr);
    SpokeWire(const SpokeWire &) = default;
    SpokeWire(SpokeWire &&) = default;
    SpokeWire & operator=(const SpokeWire &) = default;
    ~SpokeWire();


private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    const qreal POS_Y;
};

#endif // SPOKEWIRE_H
