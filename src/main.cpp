#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "backtester.hpp"
#include "candlestick.hpp"
#include "strategy.hpp"

std::vector<Candlestick> load_csv_data(const std::string& filepath) {
    std::vector<Candlestick> candles;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file.");
    }
    std::string line;
    std::getline(file, line);
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        Candlestick temp_candle;
        std::getline(ss, cell, ',');
        temp_candle.timestamp = std::stoll(cell);
        std::getline(ss, cell, ',');
        temp_candle.open = std::stod(cell);
        std::getline(ss, cell, ',');
        temp_candle.high = std::stod(cell);
        std::getline(ss, cell, ',');
        temp_candle.low = std::stod(cell);
        std::getline(ss, cell, ',');
        temp_candle.close = std::stod(cell);
        candles.push_back(temp_candle);
    }
    file.close();
    return candles;
}

int main() {

    try {
        std::cout << "Setting up experiment..." << std::endl;
        const std::string& filepath = "data/BTC-USD.csv";
        std::vector<Candlestick> data = load_csv_data(filepath);
        const int FAST_PERIOD = 20;
        const int SLOW_PERIOD = 50;
        const double INITIAL_INVESTMENT = 10000.00;
        Strategy my_strategy(FAST_PERIOD, SLOW_PERIOD);
        Backtester my_engine(data, my_strategy);
        my_engine.run();
        my_engine.print_results(INITIAL_INVESTMENT);

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}