#pragma once

enum Type {
    NONE,
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING,
};

enum Color {
    WHITE = 8,
    BLACK = -8,
};

typedef struct Piece Piece;

struct Piece {
    enum Type type;
    enum Color color;
};

/**
 * Create a new Piece.
 *
 * @param type The type of the piece
 * @param color The color of the piece
 *
 * @return A pointer to the new piece
 */
Piece *new_piece(enum Type type, enum Color color);

/**
 * Print a piece.
 *
 * @param piece A pointer to the piece being printed
 */
void print_piece(Piece *piece);
