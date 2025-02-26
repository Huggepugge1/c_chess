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

#define QUIET_MOVE 0b0000
#define DOUBLE_PAWN_PUSH 0b0001
#define CAPTURE 0b0100
#define EN_PASSANT_CAPTURE 0b0101

/**
 * Parse a move from a string
 *
 * @param string The string to parse
 *
 * @return The move
 */
uint16_t move_from_string(char *move, Board *board);

/**
 * Convert a move to a string
 *
 * @param move The move to convert
 *
 * @return The string
 */
char *move_to_string(uint16_t move);

/**
 * Create a new move
 *
 * @param to The square to move to
 * @param from The square to move from
 * @param flags The flags of the move
 *
 * @return The move
 */
uint16_t new_move(size_t to, size_t from, uint16_t flags);

/**
 * Print a move
 *
 * @param move The move to print
 */
void print_move(uint16_t move);

/**
 * Make a move
 *
 * @param board The current position
 * @param move The move to make
 */
void make_move(Board *board, uint16_t move);

/**
 * Unmake a move
 *
 * @param board The current position
 * @param move The move to unmake
 */
void unmake_move(Board *board, uint16_t move);
