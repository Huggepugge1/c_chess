#pragma once

#include "../board.h"
#include "../vector.h"

/**
 * Generate all possible moves for the bishops
 *
 * @param board The board to generate moves for
 *
 * @returns An array of MoveVector structs containing the possible moves
 */
MoveVector *generate_bishop_moves(Board *board);
