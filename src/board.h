#pragma once

#include "piece.h"
#include "uci.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct Board Board;
typedef struct Castling Castling;

struct Castling {
    bool white_king;
    bool black_king;
    bool white_queen;
    bool black_queen;
};

struct Board {
    uint64_t white_pieces;
    uint64_t black_pieces;

    uint64_t pawns;
    uint64_t rooks;
    uint64_t knights;
    uint64_t bishops;
    uint64_t queens;
    uint64_t kings;

    Castling castling;
    size_t en_passant;

    enum Color turn;

    size_t halfmove_clock;
    size_t fullmove_number;
};

static const Board START_POS = {
    0x000000000000FFFF, // White Pieces
    0xFFFF000000000000, // Black Pieces

    0x00FF00000000FF00, // Pawns
    0x8100000000000081, // Rooks
    0x4200000000000042, // Knights
    0x2400000000000024, // Bishops
    0x0800000000000008, // Queens
    0x1000000000000010, // Kings

    {
        // Castling
        true, // White Kingsside
        true, // White Queenside
        true, // Black Kingsside
        true, // Black Queenside
    },
    64, // En Passant

    WHITE, // Turn

    0, // Halfmove Clock
    1, // Fullmove Number
};

enum BoardExportFormat {
    C,
    FEN,
};

/**
 * Convert a string to a square
 *
 * @param string The string to convert
 *
 * @return The square
 */
size_t string_to_square(char *string);

/**
 * Convert a square to a string
 *
 * @param square The square to convert
 *
 * @return The string
 */
char *square_to_string(size_t square);

/**
 * Create a new Board.
 *
 * @param fen The fen of the position, NULL if startpos
 *
 * @return A pointer to the new board
 */
Board *new_board(Split *fen);

/**
 * Check if two boards are equal.
 *
 * @param board1 The first board
 * @param board2 The second board
 *
 * @return A pointer to the new board
 */
bool board_eq(const Board *board1, const Board *board2);

/**
 * Export a board to a file.
 *
 * @param board The position to save
 * @param export_format The format of the export
 * @param name The name of the position
 * @param file The name of the file to export to
 */
void export_board(Board *board, enum BoardExportFormat export_format,
                  char *name, char *file);

/**
 * Get the piece at location square.
 *
 * @param board The position
 * @param square The square of the piece
 *
 * @return The piece on the square
 */
Piece get_piece(Board *board, size_t square);

/**
 * Print a bitmap in the form of a chessboard.
 *
 * @param bitmap The bitmap to be printed
 */
void print_bitmap(uint64_t bitmap);

/**
 * Print the current position.
 *
 * @param board The position to be printed
 */
void print_board(Board *board);
