#ifndef CALCULATION_H
#define CALCULATION_H

#include <utility>
#include <cstdint>
#include <vector>
#include <map>

class Calculation
{
public:

    static constexpr int32_t AMT_BEAM_TEN{10};
    static constexpr int32_t AMT_BEAM_QUARTER{4};
    static constexpr int32_t INDEX_QUARTER{8};

    Calculation();
    Calculation(const Calculation &) = default;
    Calculation(Calculation &&) = default;
    Calculation & operator=(const Calculation &) = default;

    static int32_t getAmtBeam(const int32_t & counter);

    double recalculateSquare(const std::pair<int32_t, int32_t> & number, bool in_calc);
    const std::vector<std::map<int32_t, bool> > & updateSquare(const double & result);

private:

    static constexpr int32_t AMT_SERIASE{12};
    static const double      MULTIPLIER[];

    int32_t        defineCountBeam(const double & value, int32_t series);
    const double & getMultiplier(int32_t index_seriase) const;
    bool   getCalcStateBeam(const int32_t & seriase, const int32_t & beam);
    double recalculateSeriase(const int32_t & seriase);
    void   resetSquare();
    void   setCalcStateBeam(const int32_t & seriase, const int32_t & beam, bool in_calc);
    void   updateSeries(double & value, const int32_t & series);

    double  _result{0};
    int32_t _amt_beam{0};
    std::vector<std::map<int32_t, bool>> _square;
};

#endif // CALCULATION_H
