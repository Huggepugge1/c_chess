#pragma once

#include "../board.h"
#include "../vector.h"

/**
 * Generate all possible moves for the bishops
 *
 * @param bishops The bitboard of the bishops
 * @param occupied The bitboard of all occupied squares
 *
 * @returns A bitboard containing the possible moves
 */
uint64_t bishop_attacks(uint64_t bishops, uint64_t occupied);

/**
 * Generate all xray attacks for a bishop
 *
 * @param occupied The bitboard of all occupied squares
 * @param blockers The bitboard of all pieces that block the bishop
 * @param bishop The bitboard of the bishop
 *
 * @returns A bitboard containing the xray attacks
 */
uint64_t xray_bishop_attacks(uint64_t occupied, uint64_t blockers,
                             uint64_t bishop);

/**
 * Generate all possible moves for the bishops
 *
 * @param board The board to generate moves for
 *
 * @returns An array of MoveVector structs containing the possible moves
 */
MoveVector *generate_bishop_moves(Board *board);
