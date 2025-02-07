#include "../src/board.h"
static const Board SICILIAN_DEFENSE_MOVE_2 = {
    0x000000001000EFFF, // White Pieces
    0xFFFB000400000000, // Black Pieces

    0x00FB00041000EF00, // Pawns
    0x8100000000000081, // Rooks
    0x4200000000000042, // Knights
    0x2400000000000024, // Bishops
    0x0800000000000008, // Queens
    0x1000000000000010, // Kings

    {
        // Castling
        true, // White King side
        true, // White Queen side
        true, // Black King side
        true, // Black Queen side
    },

    42,    // En Passant
    WHITE, // Turn
    0,     // Halfmove Clock
    2,     // Fullmove Number
};
#include "../src/board.h"
static const Board SICILIAN_DEFENSE_MOVE_3 = {
    0x000000001020EFBF, // White Pieces
    0xFFFB000400000000, // Black Pieces

    0x00FB00041000EF00, // Pawns
    0x8100000000000081, // Rooks
    0x4200000000200002, // Knights
    0x2400000000000024, // Bishops
    0x0800000000000008, // Queens
    0x1000000000000010, // Kings

    {
        // Castling
        true, // White King side
        true, // White Queen side
        true, // Black King side
        true, // Black Queen side
    },

    64,    // En Passant
    BLACK, // Turn
    1,     // Halfmove Clock
    2,     // Fullmove Number
};
