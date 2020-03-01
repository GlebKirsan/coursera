#pragma once

#include <chrono>
#include <ostream>
#include <iostream>
#include <string>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;


class LogDuration {
public:
    explicit LogDuration(const std::string& msg = "")
            : message(msg + ": ")
            , start(steady_clock::now())
    {
    }

    ~LogDuration() {
        auto finish = steady_clock::now();
        auto dur = finish - start;
        std::cerr << message
                  << duration_cast<milliseconds>(dur).count()
                  << " ms" << std::endl;
    }
private:
    std::string message;
    steady_clock::time_point start;
};

#define UNIQ_ID_IMPL(lineno) _a_local_var_##lineno
#define UNIQ_ID(lineno) UNIQ_ID_IMPL(lineno)

#define LOG_DURATION(message) LogDuration UNIQ_ID(__LINE__){message};