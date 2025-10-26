# C++ Probabilistic Trading Backtester

This project is a C++ "laboratory" designed to model a financial trading strategy as a probabilistic experiment. The goal is not to build a live trading bot, but to apply concepts from applied probability to a real-world dataset.

This program simulates a Simple Moving Average (SMA) Crossover strategy on historical Bitcoin (BTC-USD) data to answer a central question: **"Does this strategy have a positive expected value (**$E[X] > 0$**)?"**

The profit/loss of each trade is treated as a discrete random variable ($X$), and this program calculates its key properties:

* **Expected Value (**$E[X]$**):** The average profit/loss per trade.

* **Variance (**$\text{Var}(X)$**):** The risk or volatility of the strategy.

* **Probability of a Win (**$P(X > 0)$**):** The win rate.

## How It Works

The project is built in C++17 and uses a modular, object-oriented design:

* **`Strategy` class:** Encapsulates the trading rules (a 20-period fast SMA and a 50-period slow SMA). It's a "state machine" that detects "golden cross" (BUY) and "death cross" (SELL) signals.

* **`Backtester` class:** The simulation engine. It loads all historical data, feeds it to the `Strategy` one candle at a time, simulates executing trades, and records the percentage return of every closed trade.

* **`main.cpp`:** The entry point. It loads the data, sets up the objects, runs the simulation, and prints the final statistical analysis.

## Getting Started

### 1. Prerequisites

* A C++17 compiler (e.g., GCC, Clang, or MSVC)

* [CMake](https://cmake.org/download/) (version 3.15 or higher)

* [Python 3](https://www.python.org/downloads/) (for the data downloader)

* Python libraries: `pandas` and `yfinance`

### 2. Get the Data

This project requires a `BTC-USD.csv` file formatted with `Timestamp,Open,High,Low,Close`. A Python script is included to download this data for you.