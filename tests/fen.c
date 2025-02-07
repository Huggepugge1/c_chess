#include "../src/board.h"
#include "../src/uci.h"
#include "../test_positions/test_positions.c"
#include "test_framework.h"

#include <string.h>

TEST(load_startpos) {
    Board *board = new_board(NULL);
    ASSERT(board_eq(board, (Board *)&START_POS));
    free(board);
}

TEST(load_fen) {
    char *fen_string = calloc(100, sizeof(char));
    strcpy(fen_string,
           "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2");
    Split *fen = split(fen_string);
    Board *board = new_board(fen);

    ASSERT(board_eq(board, &SICILIAN_DEFENSE_MOVE_2));

    free_split(fen);
    free(board);

    strcpy(fen_string,
           "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
    fen = split(fen_string);
    board = new_board(fen);

    ASSERT(board_eq(board, &SICILIAN_DEFENSE_MOVE_3));

    free_split(fen);
    free(board);

    free(fen_string);
}
