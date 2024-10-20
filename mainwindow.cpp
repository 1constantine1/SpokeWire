#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "spokewire.h"

#include <QMouseEvent>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    this->resize(1200, 800);
    this->setFixedSize(1200, 800);
    _ui->lcdNumber->resize(600, 50);
    _ui->lcdNumber->setFixedSize(600, 50);
    _ui->graphicsView->resize(600, 610);
    _ui->graphicsView->setFixedSize(600, 610);
    _ui->graphicsView->setScene(installSceneParameters());
    _ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    _ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    _ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
}

MainWindow::~MainWindow()
{
    delete _scene;
    _scene = nullptr;
    delete _ui;
    _ui = nullptr;
}

QGraphicsScene * MainWindow::installSceneParameters()
{
    QPalette palette;
    _scene = new QGraphicsScene();
    _scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    _scene->setSceneRect(0, 0, 580, 590);
    _scene->setPalette(palette);

    for (auto series = 0; series < AMT_SERIES; ++series)
        installSeriesParameters(series);
    return _scene;
}

void MainWindow::installSeriesParameters(const int32_t & series)
{
    _scene->addItem(new SpokeWire{series * TOP_BEAD + OFFSET_CENTER});
    qreal   left{START_SPOKE_WIRE};
    qreal   right{END_SPOKE_WIRE};
    Bead *  neighbor_right{nullptr};
    Bead *  move{nullptr};
    auto    amt_bead{Calculation::getAmtBeam(series)};
    qreal   offset_quarter{((amt_bead == AMT_BEAD_QUARTER) ? WIDTH_BEAD_QUARTER : 0)};

    for (auto bead = amt_bead; bead >= 1; --bead)
        neighbor_right = installBeadParameters(bead, series, left, right, neighbor_right, move, offset_quarter);
}

Bead *MainWindow::installBeadParameters(const int32_t & bead,  const int32_t & series, qreal & left, qreal & right,
                                       Bead * neighbor_right, Bead * move, const qreal & offset_quarter)
{
    left  -= WIDTH_BEAD;
    right -= WIDTH_BEAD;
    move = new Bead(std::make_pair(series, bead), (left - offset_quarter), right, neighbor_right);
    move->setPos(right, series * TOP_BEAD);
    if (neighbor_right != nullptr)
        neighbor_right->setNeighborLeft(move);
    neighbor_right = move;
    _scene->addItem(move);

    connect(move,
            SIGNAL(signalReleaseEventBead(const std::pair<int32_t, int32_t> &, bool)),
            this,
            SLOT(installValue(const std::pair<int32_t, int32_t> &, bool)));

    _moves.insert({std::make_pair(series, bead), move});
    return neighbor_right;
}

void MainWindow::installValue(const std::pair<int32_t, int32_t> & number, bool in_calc)
{
    _value_current = _calc.recalculateSquare(number, in_calc);
    _ui->lcdNumber->display(_value_current);
}

void MainWindow::on_pb_minus_clicked()
{
    _value_first = _value_current;
    _is_minus = true;
    _is_plus  = false;
}

void MainWindow::on_pb_plus_clicked()
{
    _value_first = _value_current;
    _is_minus = false;
    _is_plus  = true;
}

void MainWindow::on_pb_equally_clicked()
{
    _value_second = _value_current;
    if (_is_minus)
    {
        _result = _value_first - _value_second;
        updateSquare(_result);
    } else if (_is_plus)
    {
        _result = _value_first + _value_second;
        updateSquare(_result);
    }
    _is_minus = false;
    _is_plus  = false;
}

void MainWindow::updateSquare(const double & result)
{
    const std::vector<std::map<int32_t, bool>> & square = _calc.updateSquare(result);
    for (auto series = 0; series < AMT_SERIES; ++series)
        updateSeries(series, square);
}

void MainWindow::updateSeries(int32_t series, const std::vector<std::map<int32_t, bool>> & square)
{
    auto amt_bead{Calculation::getAmtBeam(series)};
    for (auto beam = 1; beam <= amt_bead; ++beam)
    {
        _moves.at(std::make_pair(series, beam))->setInCalc(square.at(static_cast<uint32_t>(series)).at(beam));
        _moves.at(std::make_pair(series, beam))->updatePos();
    }
}
