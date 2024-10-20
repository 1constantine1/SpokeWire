#ifndef BEAD_H
#define BEAD_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <cstdint>

class Bead : public QObject, public QGraphicsItem
{

    Q_OBJECT

public:

    explicit Bead(const std::pair<int32_t, int32_t> & number,
                  const qreal & left,
                  const qreal & right,
                  Bead * neighbor_right,
                  QObject *parent = nullptr);
    Bead(const Bead &) = default;
    Bead(Bead &&) = default;
    Bead & operator=(const Bead &) = default;
    ~Bead();

    bool getInCalc() const { return _in_calc; }
    void setNeighborLeft(Bead * neighbor_left) {_neighbor_left = neighbor_left; }
    void setInCalc(bool in_calc) { _in_calc = in_calc; }
    void updatePos();

signals:

    void signalReleaseEventBead(const std::pair<int32_t, int32_t> & number, bool in_calc);

private:

    enum class StateBead : int32_t
    {
        START  = 0,  ///< Первичное состояние Bead
        NORMAL = 1,  ///< Нормальное состояние Bead
        PRESS  = 2   ///< Cостояние, в котором нажата Bead
    };

    constexpr static int32_t BORDERS{40};
    constexpr static int32_t HIGHT_BEAD_NORMAL{40};
    constexpr static int32_t HIGHT_BEAD_PRESS{38};
    constexpr static int32_t WIDTH_BEAD_NORMAL{30};
    constexpr static int32_t WIDTH_BEAD_PRESS{28};

    QRectF boundingRect() const;
    void demarcate(qreal &x);
    void installBorders(qreal & x, QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setPosByX(const qreal & x);

    const std::pair<int32_t, int32_t> _number;
    qreal     _left;
    qreal     _right;
    Bead *    _neighbor_right;
    Bead *    _neighbor_left;
    bool      _in_calc;
    StateBead _pressed;
};

#endif // BEAD_H
