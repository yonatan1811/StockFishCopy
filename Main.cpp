#include <iostream>
#include <string>
#include "lichess_api.h"
#include "stockfish_interface.h"

int main() {
    // Initialize Lichess API and Stockfish
    LichessAPI lichess("lip_8J9j8BmivuoA1h0sPFwL"); // Replace with your actual token
    StockfishInterface stockfish("/stockfish.exe"); // Set path to Stockfish binary

    // Start listening for games
    lichess.startListening([&lichess ,&stockfish](const std::string& game_id, const std::string& moves) {
        // Set the position for Stockfish
        stockfish.sendPosition(moves);

        // Get the best move from Stockfish
        std::string best_move = stockfish.getBestMove();

        // Send the move back to Lichess
        lichess.sendMove(game_id, best_move);
    });

    // Keep the main thread alive
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
