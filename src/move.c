#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "piece.h"

uint16_t parse_move(char *string) {
    uint16_t from = string_to_square(string);
    uint16_t to = string_to_square(string + 2);
    uint16_t flags = 0;

    if (strlen(string) == 5) {
        switch (string[4]) {
        case 'r':
            flags = 0b1000;
            break;

        case 'n':
            flags = 0b1001;
            break;

        case 'b':
            flags = 0b1010;
            break;

        case 'q':
            flags = 0b1011;
            break;

        default:
            break;
        }
    }

    return from | (to << 6) | (flags << 12);
}

uint16_t annotate_move(uint16_t move, Board *board) {
    size_t from = move & 0b111111;
    size_t to = (move >> 6) & 0b111111;
    size_t flags = (move >> 12) & 0b1111;

    Piece from_piece = get_piece(board, from);
    Piece to_piece = get_piece(board, to);

    if (board->turn == WHITE && to_piece.type != NONE &&
        to_piece.color == BLACK) {
        flags |= 0b0100;
    } else if (board->turn == BLACK && to_piece.type != NONE &&
               to_piece.color == WHITE) {
        flags |= 0b0100;
    } else if (from_piece.type == PAWN) {
        if (abs((int)from - (int)to) == 16) {
            flags |= 0b0001;
        } else if (to == board->en_passant) {
            flags |= 0b0101;
        } else if (to / 8 == 0 || to / 8 == 7) {
            flags |= 0b1000;
        }
    } else if (from_piece.type == KING) {
        if (from == 4) {
            if (to == 6) {
                flags |= 0b0010;
            } else if (to == 2) {
                flags |= 0b0011;
            }
        } else if (from == 60) {
            if (to == 62) {
                flags |= 0b0010;
            } else if (to == 58) {
                flags |= 0b0011;
            }
        }
    }
    return move | (flags << 12);
}

uint16_t move_from_string(char *string, Board *board) {
    uint16_t move = parse_move(string);
    return annotate_move(move, board);
}

void print_move(uint16_t move) {
    printf("Move: %x\n", move);
    uint16_t from = move & 0b111111;
    uint16_t to = (move >> 6) & 0b111111;
    uint16_t flags = (move >> 12) & 0b1111;

    char *from_str = square_to_string(from);
    char *to_str = square_to_string(to);

    printf("%s%s", from_str, to_str);
    if (flags & 0b1000) {
        if ((flags & 0b0011) == 0b0000) {
            printf("r");
        } else if ((flags & 0b0011) == 0b0001) {
            printf("n");
        } else if ((flags & 0b0011) == 0b0010) {
            printf("b");
        } else if ((flags & 0b0011) == 0b0011) {
            printf("q");
        }
    }

    printf("\n");

    free(from_str);
    free(to_str);
}
