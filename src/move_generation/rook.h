#pragma once

#include "../board.h"
#include "../vector.h"

/**
 * Generate all possible moves for the rooks
 *
 * @param rooks The bitboard of the rooks
 * @param own_pieces The bitboard of the pieces of the player whose turn it is
 * @param enemy_pieces The bitboard of the pieces of the player whose turn it is
 *
 * @returns A bitboard containing the possible moves
 */
uint64_t rook_attacks(uint64_t rooks, uint64_t own_pieces,
                      uint64_t enemy_pieces);

/**
 * Generate all possible moves for the rooks
 *
 * @param board The board to generate moves for
 *
 * @returns An array of MoveVector structs containing the possible moves
 */
MoveVector *generate_rook_moves(Board *board);
