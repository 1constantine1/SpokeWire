#include "bead.h"

#include <QPointF>
#include <cmath>

Bead::Bead(const std::pair<int32_t, int32_t> &number,
           const qreal & left,
           const qreal & right,
           Bead * neighbor_right,
           QObject *parent) :
    QObject{parent},
    QGraphicsItem{},
    _number{number},
    _left{left},
    _right{right},
    _neighbor_right{neighbor_right},
    _neighbor_left{nullptr},
    _in_calc{false},
    _pressed{StateBead::START}
{}

Bead::~Bead()
{}

QRectF Bead::boundingRect() const
{
    return QRectF(0, 0, WIDTH_BEAD_NORMAL, HIGHT_BEAD_NORMAL);
}

void Bead::demarcate(qreal &x)
{
    if (_neighbor_right != nullptr)
        if (x > _neighbor_right->pos().x() - BORDERS)
            x = _neighbor_right->pos().x() - BORDERS;
    if (_neighbor_left != nullptr)
        if (x < _neighbor_left->pos().x() + BORDERS)
            x = _neighbor_left->pos().x() + BORDERS;
}

void Bead::installBorders(qreal & x, QGraphicsSceneMouseEvent *event)
{
    if (x > _right)
    {
        x = _right;
        event->setPos(event->lastPos());
    }
    else
    {
        if (x < _left)
        {
            x = _left;
            event->setPos(event->lastPos());
        }
    }
}

void Bead::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    _pressed = StateBead::PRESS;
    qreal x{mapToScene(event->pos()).x()};
    installBorders(x, event);
    demarcate(x);
    _in_calc = (std::lrint(_left) == std::lrint(x));
    setPosByX(x);
    update();
}

void Bead::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    _pressed = StateBead::PRESS;
    this->setCursor(QCursor(Qt::ClosedHandCursor));
    update();
    Q_UNUSED(event);
}

void Bead::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    _pressed = StateBead::NORMAL;
    this->setCursor(QCursor(Qt::ArrowCursor));
    update();
    emit signalReleaseEventBead(_number, _in_calc);
    Q_UNUSED(event);
}

void Bead::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    switch (_pressed) {
    case StateBead::START:
    {
        this->setPos(_right, this->pos().y());
        painter->setBrush(Qt::gray);
        painter->drawRect(0, 0, WIDTH_BEAD_NORMAL, HIGHT_BEAD_NORMAL);
        _pressed = StateBead::NORMAL;
        break;
    }
    case StateBead::NORMAL:
    {
        _in_calc ? painter->setBrush(Qt::green) : painter->setBrush(Qt::gray);
        painter->drawRect(0, 0, WIDTH_BEAD_NORMAL, HIGHT_BEAD_NORMAL);
        break;
    }
    case StateBead::PRESS:
    {
        painter->setBrush(Qt::red);
        painter->drawRect(0, 0, WIDTH_BEAD_PRESS, HIGHT_BEAD_PRESS);
        break;
    }
    default:
    {
        break;
    }
    }
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Bead::setPosByX(const qreal & x)
{
    QPointF pointf;
    pointf.setX(x);
    pointf.setY(this->pos().y());
    this->setPos(pointf);
}

void Bead::updatePos()
{
    setPosByX(_in_calc ? _left : _right);
    this->mouseReleaseEvent(nullptr);
}

