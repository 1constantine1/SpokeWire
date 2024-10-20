#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "bead.h"
#include "calculation.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <cstdint>
#include <utility>
#include <map>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    MainWindow(const MainWindow &) = default;
    MainWindow(MainWindow &&) = default;
    MainWindow & operator=(const MainWindow &) = default;
    ~MainWindow();

public slots:

    void installValue(const std::pair<int32_t, int32_t> & number, bool in_calc);

private:

    constexpr static int32_t AMT_SERIES{12};
    constexpr static int32_t AMT_BEAD_QUARTER{4};
    constexpr static int32_t INDEX_QUARTER{8};

    constexpr static qreal START_SPOKE_WIRE{400};
    constexpr static qreal END_SPOKE_WIRE{590};
    constexpr static qreal WIDTH_BEAD{40};
    constexpr static qreal WIDTH_BEAD_QUARTER{6 * 40};
    constexpr static qreal TOP_BEAD{50};
    constexpr static qreal OFFSET_CENTER{20};

    QGraphicsScene * installSceneParameters();
    void installSeriesParameters(const int32_t &series);
    Bead* installBeadParameters(const int32_t & bead, const int32_t &series, qreal &left,
                               qreal &right, Bead *neighbor_right, Bead *move, const qreal &offset_quarter);
    void updateSquare(const double & result);
    void updateSeries(int32_t series, const std::vector<std::map<int32_t, bool> > &square);

    Ui::MainWindow * _ui{nullptr};
    QGraphicsScene * _scene{nullptr};
    std::map<std::pair<int32_t, int32_t>, Bead *> _moves;
    Calculation _calc;
    double      _value_current{0};
    double      _value_first{0};
    double      _value_second{0};
    double      _result{0};
    bool        _is_minus{false};
    bool        _is_plus{false};

protected:

private slots:

    void on_pb_minus_clicked();
    void on_pb_plus_clicked();
    void on_pb_equally_clicked();
};

#endif // MAINWINDOW_H
