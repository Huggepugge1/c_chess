#pragma once

#include "../board.h"
#include "../vector.h"

/**
 * Generate all possible moves for the rooks
 *
 * @param rooks The bitboard of the rooks
 * @param occupied The bitboard of all occupied squares
 *
 * @returns A bitboard containing the possible moves
 */
uint64_t rook_attacks(uint64_t rooks, uint64_t occupied);

/**
 * Generate all xray attacks for a rook
 *
 * @param occupied The bitboard of all occupied squares
 * @param blockers The bitboard of all pieces that block the rook
 * @param square The square the rook is on
 *
 * @returns A bitboard containing the xray attacks
 */
uint64_t xray_rook_attacks(uint64_t occupied, uint64_t blockers, size_t square);

/**
 * Generate all possible moves for the rooks
 *
 * @param board The board to generate moves for
 *
 * @returns An array of MoveVector structs containing the possible moves
 */
MoveVector *generate_rook_moves(Board *board);
