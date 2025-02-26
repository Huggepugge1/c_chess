#pragma once

#include "../board.h"
#include "../vector.h"

/**
 * Get all knight attacks
 *
 * @param knights The bitboard of the knights
 *
 * @return A bitboard containing all possible knight attacks
 */
uint64_t knight_attacks(uint64_t knights);

/**
 * Precompute all possible knight attacks
 */
void generate_knight_attack_bitboards();

/**
 * Generate all possible moves for the knights
 *
 * @param board The board to generate moves for
 *
 * @returns An array of MoveVector structs containing the possible moves
 */
MoveVector *generate_knight_moves(Board *board);
