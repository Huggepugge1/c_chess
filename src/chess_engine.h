#pragma once

#include "board.h"
#include <stdbool.h>

typedef struct ChessEngine ChessEngine;

struct ChessEngine {
    Board *board;
    bool debug;
};

/**
 * Create a new chess engine.
 *
 * @return A pointer to a chess engine
 */
ChessEngine *new_chess_engine();

/**
 * Free a chess engine.
 *
 * @param The chess engine to be freed
 */
void free_chess_engine(ChessEngine *chess_engine);
