#include "../src/chess_engine.h"
#include "../src/board.h"
#include "test_framework.h"

TEST(chess_engine_allocation) {
    ChessEngine *chess_engine = new_chess_engine();
    ASSERT_NOT_NULL(chess_engine);
    ASSERT_EQ(chess_engine->debug, 0);
    ASSERT(board_eq(chess_engine->board, &START_POS))
    free_chess_engine(chess_engine);
}
