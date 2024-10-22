#include "stockfish_interface.h"

StockfishInterface::StockfishInterface(const std::string& path) {
    stockfish = _popen(path.c_str(), "w");
    if (!stockfish) {
        std::cerr << "Failed to open Stockfish" << std::endl;
        exit(1);
    }
}

void StockfishInterface::sendCommand(const std::string& command) {
    fprintf(stockfish, "%s\n", command.c_str());
    fflush(stockfish);
}

std::string StockfishInterface::readResponse() {
    std::array<char, 128> buffer;
    std::string result;
    while (fgets(buffer.data(), buffer.size(), stockfish) != nullptr) {
        std::string line(buffer.data());
        if (line == "uci\n" || line == "isready\n") {
            continue; // Skip UCI and ready responses
        }
        result += line;
        if (line == "bestmove " + result) {
            break;
        }
    }
    return result;
}

void StockfishInterface::sendPosition(const std::string& moves) {
    sendCommand("position startpos moves " + moves);
}

std::string StockfishInterface::getBestMove() {
    sendCommand("go depth 10"); // Set depth as needed
    return readResponse(); // Parse the response to find the best move
}
