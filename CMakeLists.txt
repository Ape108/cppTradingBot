# --- 1. Project Setup ---
# Specifies the minimum version of CMake required to build this project.
cmake_minimum_required(VERSION 3.15)

# Defines the project name and sets the programming language.
project(TradingBot CXX)

# Sets the C++ standard to C++17. This enables modern language features.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# --- 2. Defining the Executable ---
# Tells CMake to create an executable named "trading_bot" from these source files.
add_executable(trading_bot
    src/main.cpp
    src/strategy.cpp
    src/backtester.cpp
)

# Tells the compiler to look for header files (.hpp) in the "include" directory.
# This is necessary because your .cpp files use #include "header.hpp".
target_include_directories(trading_bot PUBLIC include)


# --- 3. Managing Dependencies (External Libraries) ---
# This section automatically downloads and prepares the libraries your bot needs
# for networking (cpr) and handling JSON data (nlohmann/json).
include(FetchContent)

# Declare the CPR library for making HTTP requests.
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.10.5
)

# Declare the nlohmann/json library for parsing JSON.
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

# Make the libraries available for use.
FetchContent_MakeAvailable(cpr nlohmann_json)


# --- 4. Linking Libraries ---
# This is the final and most crucial step. It connects your executable
# with the libraries it depends on. Without this, you'll get linker errors.
target_link_libraries(trading_bot PUBLIC
    cpr::cpr
    nlohmann_json::nlohmann_json
)