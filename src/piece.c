#include "piece.h"

#include <ctype.h>
#include <stdio.h>

Piece new_piece(enum Type type, enum Color color) {
    return (Piece){
        type,
        color,
    };
}

char piece_to_char(Piece piece) {
    char c;
    switch (piece.type) {
    case NONE:
        c = '.';
        break;
    case PAWN:
        c = 'p';
        break;
    case ROOK:
        c = 'r';
        break;
    case KNIGHT:
        c = 'n';
        break;
    case BISHOP:
        c = 'b';
        break;
    case QUEEN:
        c = 'q';
        break;
    case KING:
        c = 'k';
        break;
    }
    if (piece.color == WHITE) {
        c = toupper(c);
    }
    return c;
}

void print_piece(Piece piece) {
    printf("%c", piece_to_char(piece));
}
