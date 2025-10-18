//
// Created by SAMANYU on 18-10-2025.
//

#ifndef JSONPARSER_EXECUTIONTIMER_H
#define JSONPARSER_EXECUTIONTIMER_H

#include <chrono>
#include <sstream>

class ExecutionTimer
{
    public:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
    std::chrono::duration<double> elapsed{};
    std::chrono::duration<double> total{};
    bool restoreCoutStream;

    ExecutionTimer(): ExecutionTimer(false) {};

    ExecutionTimer(const bool &restoreCout)
    {
        start = std::chrono::high_resolution_clock::now();
        restoreCoutStream = restoreCout;
    }
    ~ExecutionTimer()
    {
        end = std::chrono::high_resolution_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        std::stringstream ss;
        ss.copyfmt(std::cout);
        std::cout << std::fixed << std::setprecision(10);
        std::cout << "Execution time: " << elapsed.count() << "s" << std::endl;
        if (restoreCoutStream)
            std::cout.copyfmt(ss);
    }

};

#endif //JSONPARSER_EXECUTIONTIMER_H
