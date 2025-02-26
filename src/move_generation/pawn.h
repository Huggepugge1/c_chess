#include "../board.h"
#include "../piece.h"
#include "../vector.h"

/**
 * Get all pawn attacks
 *
 * @param pawns The bitboard of the pawns
 * @param color The color of the player whose turn it is
 *
 * @return An array of MoveVector structs containing the possible moves
 */
uint64_t pawn_attacks(uint64_t pawns, enum Color color);

/**
 * Generate all possible pawn moves.
 *
 * @return A MoveVector containing all possible pawn moves
 */
MoveVector *generate_pawn_moves(Board *board);
