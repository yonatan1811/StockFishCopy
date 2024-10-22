#ifndef STOCKFISH_INTERFACE_H
#define STOCKFISH_INTERFACE_H

#include <string>
#include <iostream>
#include <memory>
#include <cstdio>
#include <array>

class StockfishInterface {
public:
    StockfishInterface(const std::string& path);
    void sendPosition(const std::string& moves);
    std::string getBestMove();

private:
    FILE* stockfish;
    void sendCommand(const std::string& command);
    std::string readResponse();
};

#endif // STOCKFISH_INTERFACE_H
