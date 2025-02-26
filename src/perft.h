/**
 * Runs the perft test on the board.
 *
 * @param board The board to run the test on
 * @param depth The depth to run the test to
 *
 * @return The number of nodes at the given depth
 */
#include "board.h"
#include "chess_engine.h"
#include <stddef.h>

typedef struct PerftArgs PerftArgs;

struct PerftArgs {
    ChessEngine *chess_engine;
    size_t depth;
};

/**
 * Runs the perft test on the board.
 *
 * @param args The arguments to the perft function
 *
 * @return The number of nodes at the given depth
 */
void *perft(void *args);
