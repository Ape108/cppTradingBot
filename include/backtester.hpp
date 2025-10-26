#pragma once

#include <vector>
#include "candlestick.hpp"
#include "strategy.hpp"

class Backtester {
public: 
    Backtester(const std::vector<Candlestick> data, Strategy& strat);
    void run();
    void print_results();

private:
    std::vector<Candlestick> historical_data;
    Strategy& strategy;
    bool is_position_open;
    double entry_price;
    std::vector<double> trade_returns;

    double calculate_mean();
    double calculate_variance(double mean);
    double calculate_win_rate();

};