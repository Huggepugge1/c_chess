#include "piece.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

Piece *new_piece(enum Type type, enum Color color) {
    Piece *result = malloc(sizeof(Piece));

    result->type = type;
    result->color = color;

    return result;
}

void print_piece(Piece *piece) {
    char c;
    switch (piece->type) {
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
    if (piece->color == WHITE) {
        c = toupper(c);
    }
    printf("%c", c);
}
