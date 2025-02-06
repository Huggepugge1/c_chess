#include "chess_engine.h"
#include "board.h"
#include "exitcode.h"

#include <stdlib.h>

ChessEngine *new_chess_engine() {
    ChessEngine *chess_engine = calloc(1, sizeof(ChessEngine));
    if (!chess_engine) {
        exit(EXITCODE_CHESS_ENGINE_ALLOCATION_FAILED);
    }
    chess_engine->board = new_board(NULL);
    return chess_engine;
}

void free_chess_engine(ChessEngine *chess_engine) {
    free(chess_engine->board);
    free(chess_engine);
}
