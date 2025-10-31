#include "strategy.hpp"
#include <numeric>

Strategy::Strategy(int fast_sma_period, int slow_sma_period)
    : fast_period(fast_sma_period),
      slow_period(slow_sma_period),
      current_fast_sma(0.0),
      current_slow_sma(0.0) 
{
    
}

Signal Strategy::process_candlestick(const Candlestick& candle) {

    const double last_fast_sma = this->current_fast_sma;
    const double last_slow_sma = this->current_slow_sma;

    recent_prices.push_back(candle.close);

    double oldest_slow_price = 0.0;
    if (recent_prices.size() > slow_period) {
        oldest_slow_price = recent_prices.front();
        recent_prices.pop_front();
    }

    if (recent_prices.size() < slow_period) {
        return Signal::HOLD;
    }

    const double newest_price = candle.close;
    if (current_slow_sma == 0.0) { // Run only the first time -- O(k)
        double slow_sum = 0.0;
        double fast_sum = 0.0;

        for (size_t i = 0; i < recent_prices.size(); ++i) {
            slow_sum += recent_prices[i];
            if (i >= slow_period - fast_period) {
                fast_sum += recent_prices[i];
            }
        }

        this->current_fast_sma = fast_sum / fast_period;
        this->current_slow_sma = slow_sum / slow_period;
    }
    else { // O(1) -- Recurrence relation
        double oldest_fast_price = recent_prices[slow_period - fast_period];
        this->current_fast_sma = last_fast_sma + (newest_price - oldest_fast_price) / fast_period;
        this->current_slow_sma = last_slow_sma + (newest_price - oldest_slow_price) / slow_period;
    }

    if (last_fast_sma < last_slow_sma && current_fast_sma > current_slow_sma) {
        return Signal::BUY;
    } else if (last_fast_sma > last_slow_sma && current_fast_sma < current_slow_sma) {
        return Signal::SELL;
    }

    return Signal::HOLD;

}