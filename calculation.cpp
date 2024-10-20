#include "calculation.h"

#include <cmath>
#include <utility>

const double Calculation::MULTIPLIER[] = { 10000000.0, 1000000.0, 100000.0,
                                           10000.0,    1000.0,    100.0,
                                           10.0,       1.0,       0.25,
                                           0.1,        0.01,      0.001 };

Calculation::Calculation()
{
    resetSquare();
}

int32_t Calculation::defineCountBeam(const double & value, int32_t series)
{
    return static_cast<int32_t>(std::lrint(std::trunc(value / (getMultiplier(series)))));
}

int32_t Calculation::getAmtBeam(const int32_t & counter)
{
    return (counter == INDEX_QUARTER) ? AMT_BEAM_QUARTER : AMT_BEAM_TEN;
}

bool Calculation::getCalcStateBeam(const int32_t & seriase, const int32_t & beam)
{
    return _square.at(static_cast<uint32_t>(seriase)).at(beam);
}

const double & Calculation::getMultiplier(int32_t index_seriase) const
{
    return MULTIPLIER[index_seriase];
}

void Calculation::resetSquare()
{
    _square.clear();
    std::map<int32_t, bool> seriases;
    for (auto seriase = 0; seriase < AMT_SERIASE; ++seriase)
    {
        _amt_beam = getAmtBeam(seriase);
        for (auto beam = 1; beam <= _amt_beam; ++beam)
            seriases.insert({beam, false});
        _square.push_back(seriases);
        seriases.clear();
    }
}

double Calculation::recalculateSquare(const std::pair<int32_t, int32_t> &number, bool in_calc)
{
    _result   = 0;
    _amt_beam = AMT_BEAM_TEN;
    setCalcStateBeam(number.first, number.second, in_calc);
    for (auto seriase = 0; seriase < AMT_SERIASE; ++seriase)
        _result += recalculateSeriase(seriase);
    return _result;
}

double Calculation::recalculateSeriase(const int32_t & seriase)
{
    double count{0.0};
    _amt_beam = getAmtBeam(seriase);
    for (auto beam = 1; beam <= _amt_beam; ++beam)
    {
        if (getCalcStateBeam(seriase, beam))
            count += 1.0;
    }
    return count * getMultiplier(seriase);
}

void Calculation::setCalcStateBeam(const int32_t & seriase, const int32_t & beam, bool in_calc)
{
    _square.at(static_cast<uint32_t>(seriase)).at(beam) = in_calc;
}

const std::vector<std::map<int32_t, bool>> & Calculation::updateSquare(const double & result)
{
    resetSquare();
    double temp{result};
    for (auto series = 0; series < AMT_SERIASE ; ++series)
        updateSeries(temp, series);
    return _square;
}

void Calculation::updateSeries(double & value, const int32_t & series)
{
    int32_t count_beam{defineCountBeam(value, series)};
    if ((count_beam != 0) && (count_beam <= AMT_BEAM_TEN))
    {
        value -= count_beam * getMultiplier(series);
        for (auto beam = 1; beam <= count_beam ; ++beam)
            setCalcStateBeam(series, beam, true);
    }
}
