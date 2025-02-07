// MOVE: FLAG | TO | FROM
// Bits: 4    | 6  | 6
//
// FLAGS:
// promotion | capture | special 1 | special 2
// 0b0000 quiet
// 0b0001 double pawn push
// 0b0010 king castle
// 0b0011 quuen castle
// 0b0100 captures
// 0b0101 en passant capture
// 0b1000 rook promotion
// 0b1001 knight promotion
// 0b1010 bishop promotion
// 0b1011 queen promotion
// 0b1100 rook promotion capture
// 0b1101 knight promotion capture
// 0b1110 bishop promotion capture
// 0b1111 queen promotion capture

#include "board.h"

#include <stdint.h>

/**
 * Parse a move from a string
 *
 * @param string The string to parse
 *
 * @return The move
 */
uint16_t move_from_string(char *move, Board *board);
