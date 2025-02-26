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

extern const Piece WHITE_PAWN;
extern const Piece WHITE_ROOK;

extern const Piece BLACK_PAWN;
extern const Piece BLACK_ROOK;

/**
 * Create a new Piece.
 *
 * @param type The type of the piece
 * @param color The color of the piece
 *
 * @return The new piece
 */
Piece new_piece(enum Type type, enum Color color);

/**
 * Get the char corresponding to the piece.
 *
 * @param piece A pointer to the piece being printed
 *
 * @return The char corresponding to the piece
 */
char piece_to_char(Piece piece);

/**
 * Print a piece.
 *
 * @param piece A pointer to the piece being printed
 */
void print_piece(Piece piece);
