#include "strategy.hpp"
#include <numeric>

Strategy::Strategy(int fast_sma_period, int slow_sma_period)
    : fast_period(fast_sma_period),
      slow_period(slow_sma_period) {
    last_fast_sma = 0.0;
    last_slow_sma = 0.0;
}

double Strategy::calculate_sma(int period) {
    double sum = 0.0;

    for (size_t i = 0; i < period; ++i) {
        // sums the last 'period' items.
        sum += recent_prices.at(recent_prices.size() - 1 - i);
    }

    return sum / period;
}

Signal Strategy::process_candlestick(const Candlestick& candle) {
    recent_prices.push_back(candle.close);

    if (recent_prices.size() > slow_period) {
        recent_prices.pop_front();
    }
    if (recent_prices.size() < slow_period) {
        return Signal::HOLD;
    }

    double current_fast_sma = calculate_sma(fast_period);
    double current_slow_sma = calculate_sma(slow_period);

    if (last_fast_sma < last_slow_sma && current_fast_sma > current_slow_sma) {
        // Golden Cross (BUY)
        last_fast_sma = current_fast_sma;
        last_slow_sma = current_slow_sma;
        return Signal::BUY;
    } else if (last_fast_sma > last_slow_sma && current_fast_sma < current_slow_sma) {
        // Death Cross (SELL)
        last_fast_sma = current_fast_sma;
        last_slow_sma = current_slow_sma;
        return Signal::SELL;
    }

    last_fast_sma = current_fast_sma;
    last_slow_sma = current_slow_sma;
    return Signal::HOLD;
}