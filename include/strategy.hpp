#pragma once

#include "candlestick.hpp"
#include <deque>

enum class Signal { HOLD, BUY, SELL };

class Strategy {
public:
    Strategy(int fast_sma_period, int slow_sma_period);
    Signal process_candlestick(const Candlestick& candle);

private:
    int fast_period;
    int slow_period;

    std::deque<double> recent_prices;

    double last_fast_sma;
    double last_slow_sma;

    double calculate_sma(int period);
};