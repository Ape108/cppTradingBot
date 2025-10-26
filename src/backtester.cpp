#include "backtester.hpp"
#include <iostream>
#include <iomanip>
#include <numeric>
#include <cmath>


Backtester::Backtester(const std::vector<Candlestick> data, Strategy& strat)
    : historical_data(data), 
      strategy(strat) {
    this->is_position_open = false;
    this->entry_price = 0.0;
} 

void Backtester::run() {
    trade_returns.clear();
    for (const Candlestick& candle : historical_data) {
        Signal signal = strategy.process_candlestick(candle);
        if (signal == Signal::BUY) {
            if (is_position_open == false) {
                is_position_open = true;
                entry_price = candle.close;
            }
        } else if (signal == Signal::SELL) {
            if (is_position_open == true) {
                double exit_price = candle.close;
                double pct_return = (exit_price / entry_price) - 1.0;
                trade_returns.push_back(pct_return);
                is_position_open = false;
                entry_price = 0.0;
            }
        }
    }
}

void Backtester::print_results() {
    if (trade_returns.empty()) {
        std::cout << "No trades made." << std::endl;
        return;
    }
    double mean = calculate_mean(); // E[X]
    double variance = calculate_variance(mean); // Var(X)
    double std_dev = std::sqrt(variance); // σ_X
    double win_rate = calculate_win_rate(); // P(Win)

    std::cout << "Total Trades: " << trade_returns.size() << std::endl;
    std::cout << "Win Rate: " << std::fixed << std::setprecision(2) << (win_rate * 100) << "%" << std::endl;
    std::cout << "Expected Value: " << std::fixed << std::setprecision(2) << (mean * 100) << "%" << std::endl;
    std::cout << "Standard Deviation: " << std::fixed << std::setprecision(2) << (std_dev * 100) << "%" << std::endl;

    if (mean > 0) {
        std::cout << "The strategy has a positive expected value." << std::endl;
    }
}

double Backtester::calculate_mean() {
    double sum = std::accumulate(trade_returns.begin(), trade_returns.end(), 0.0);
    return sum / trade_returns.size();
}

double Backtester::calculate_win_rate() {
    unsigned int win_count = 0;
    for (const double& pct_return : trade_returns) {
        if (pct_return > 0) {
            win_count++;
        }
    }
    return (double)win_count / trade_returns.size();
}

double Backtester::calculate_variance(double mean) {
    double sum_of_squares = 0.0;
    for (const double& pct_return : trade_returns) {
        sum_of_squares += (pct_return - mean) * (pct_return - mean);
    }
    return sum_of_squares / (trade_returns.size() - 1);
}