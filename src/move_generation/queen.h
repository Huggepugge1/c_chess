#pragma once

#include "../board.h"
#include "../vector.h"

/**
 * Generate all possible moves for the queens
 *
 * @param queens The bitboard of the queens
 * @param occupied The bitboard of all occupied squares
 *
 * @returns A bitboard containing the possible moves
 */
uint64_t queen_attacks(uint64_t queens, uint64_t occupied);

/**
 * Generate all possible xray attacks for a queen
 *
 * @param occupied The bitboard of all occupied squares
 * @param blockers The bitboard of all pieces that block the queen
 * @param queen The bitboard of the queen
 *
 * @returns A bitboard containing the xray attacks
 */
uint64_t xray_queen_attacks(uint64_t occupied, uint64_t blockers,
                            uint64_t queen);

/**
 * Generate all possible moves for the queens
 *
 * @param board The board to generate moves for
 *
 * @returns An array of MoveVector structs containing the possible moves
 */
MoveVector *generate_queen_moves(Board *board);
