#pragma once

#include "../board.h"
#include "../vector.h"

/**
 * Get all king attacks
 *
 * @param kings The bitboard of the kings
 *
 * @return A bitboard containing all possible king attacks
 */
uint64_t king_attacks(uint64_t kings);

/**
 * Precompute all possible king attacks
 */
void generate_king_attack_bitboards();

/**
 * Generate all possible moves for the kings
 *
 * @param board The board to generate moves for
 *
 * @returns An array of MoveVector structs containing the possible moves
 */
MoveVector *generate_king_moves(Board *board);
