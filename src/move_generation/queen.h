#pragma once

#include "../board.h"
#include "../vector.h"

/**
 * Generate all possible moves for the queens
 *
 * @param board The board to generate moves for
 *
 * @returns An array of MoveVector structs containing the possible moves
 */
MoveVector *generate_queen_moves(Board *board);
