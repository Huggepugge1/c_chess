#pragma once

#include "../board.h"
#include "../vector.h"

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
