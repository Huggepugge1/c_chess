#include "../board.h"
#include "../piece.h"
#include "../vector.h"

/**
 * Generate all possible pawn moves.
 *
 * @return A MoveVector containing all possible pawn moves
 */
MoveVector *generate_pawn_moves(Board *board);
