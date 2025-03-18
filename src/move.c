#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "exitcode.h"
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

char *move_to_string(uint16_t move) {
    uint16_t from = move & 0b111111;
    uint16_t to = (move >> 6) & 0b111111;
    uint16_t flags = (move >> 12) & 0b1111;

    char *from_str = square_to_string(from);
    char *to_str = square_to_string(to);

    char *result = calloc(6, sizeof(char));
    strcpy(result, from_str);
    strcat(result, to_str);

    if (flags & 0b1000) {
        if ((flags & 0b0011) == 0b0000) {
            result[4] = 'r';
        } else if ((flags & 0b0011) == 0b0001) {
            result[4] = 'n';
        } else if ((flags & 0b0011) == 0b0010) {
            result[4] = 'b';
        } else if ((flags & 0b0011) == 0b0011) {
            result[4] = 'q';
        }
    }

    free(from_str);
    free(to_str);

    return result;
}

uint16_t new_move(size_t from, size_t to, uint16_t flags) {
    return from | (to << 6) | (flags << 12);
}

void print_move(uint16_t move) {
    char *string = move_to_string(move);
    printf("%s\n", string);
    free(string);
}

bool quiet(uint16_t flags) { return flags == 0b0000; }

bool double_pawn_push(uint16_t flags) { return flags == 0b0001; }

bool king_castle(uint16_t flags) { return flags == 0b0010; }

bool queen_castle(uint16_t flags) { return flags == 0b0011; }

bool castle(uint16_t flags) {
    return king_castle(flags) || queen_castle(flags);
}

bool capture(uint16_t flags) { return flags & 0b0100; }

bool en_passant(uint16_t flags) { return flags == 0b0101; }

bool promotion(uint16_t flags) { return flags & 0b1000; }

enum Type get_type_of_promotion(uint16_t flags) {
    switch (flags & 0b0011) {
    case 0b00:
        return ROOK;
    case 0b01:
        return KNIGHT;
    case 0b10:
        return BISHOP;
    case 0b11:
        return QUEEN;
    }
    crash(EXITCODE_UNREACHABLE);
}

void toggle_piece(Board *board, Piece piece, size_t position) {
    uint64_t bitboard = (uint64_t)1 << position;
    switch (piece.type) {
    case PAWN:
        board->pawns ^= bitboard;
        break;
    case ROOK:
        board->rooks ^= bitboard;
        break;
    case KNIGHT:
        board->knights ^= bitboard;
        break;
    case BISHOP:
        board->bishops ^= bitboard;
        break;
    case QUEEN:
        board->queens ^= bitboard;
        break;
    case KING:
        board->kings ^= bitboard;
        break;
    case NONE:
        crash(EXITCODE_EMPTY_PIECE_MODIFIED);
    default:
        crash(EXITCODE_PIECE_WITHOUT_VALID_TYPE);
    }
    switch (piece.color) {
    case WHITE:
        board->white_pieces ^= bitboard;
        break;
    case BLACK:
        board->black_pieces ^= bitboard;
        break;
    default:
        crash(EXITCODE_PIECE_WITHOUT_VALID_COLOR);
    }
}

void handle_castle(Board *board, uint16_t flags) {
    if (castle(flags)) {
        if (board->turn == WHITE) {
            if (king_castle(flags)) {
                toggle_piece(board, WHITE_ROOK, 7);
                toggle_piece(board, WHITE_ROOK, 5);
            } else if (queen_castle(flags)) {
                toggle_piece(board, WHITE_ROOK, 0);
                toggle_piece(board, WHITE_ROOK, 3);
            }
        } else if (board->turn == BLACK) {
            if (king_castle(flags)) {
                toggle_piece(board, BLACK_ROOK, 63);
                toggle_piece(board, BLACK_ROOK, 61);
            } else if (queen_castle(flags)) {
                toggle_piece(board, BLACK_ROOK, 56);
                toggle_piece(board, BLACK_ROOK, 59);
            }
        }
    }
}

void remove_castling_rights(Board *board, size_t from, size_t to,
                            Piece from_piece, uint16_t flags) {
    if (castle(flags)) {
        if (board->turn == WHITE) {
            board->castling.white_king = false;
            board->castling.white_queen = false;
        } else {
            board->castling.black_king = false;
            board->castling.black_queen = false;
        }
    }

    if (from == 0 || to == 0) {
        board->castling.white_queen = false;
    } else if (from == 7 || to == 7) {
        board->castling.white_king = false;
    } else if (from == 56 || to == 56) {
        board->castling.black_queen = false;
    } else if (from == 63 || to == 63) {
        board->castling.black_king = false;
    }

    if (from_piece.type == KING) {
        if (board->turn == WHITE) {
            board->castling.white_king = false;
            board->castling.white_queen = false;
        } else {
            board->castling.black_king = false;
            board->castling.black_queen = false;
        }
    }
}

void make_move(Board *board, uint16_t move) {
    uint16_t from = move & 0b111111;
    uint16_t to = (move >> 6) & 0b111111;
    uint16_t flags = (move >> 12) & 0b1111;

    Piece from_piece = get_piece(board, from);
    Piece to_piece = get_piece(board, to);

    Irreversible irreversible = {
        .captured_piece = to_piece,
        .halfmove_clock = board->halfmove_clock,
        .en_passant = board->en_passant,
        .castling = board->castling,
    };

    if (en_passant(flags) || capture(flags)) {
        size_t captured_piece_square =
            en_passant(flags) ? board->en_passant - board->turn : to;
        Piece captured_piece = get_piece(board, captured_piece_square);
        irreversible.captured_piece = captured_piece;
        toggle_piece(board, captured_piece, captured_piece_square);
        board->halfmove_clock = -1;
    }

    if (double_pawn_push(flags)) {
        board->en_passant = to - board->turn;
    } else {
        board->en_passant = 64;
    }

    handle_castle(board, flags);

    remove_castling_rights(board, from, to, from_piece, flags);

    if (from_piece.type == PAWN) {
        board->halfmove_clock = -1;
    }

    toggle_piece(board, from_piece, from);

    if (promotion(flags)) {
        Piece promotion_piece =
            new_piece(get_type_of_promotion(move), from_piece.color);
        toggle_piece(board, promotion_piece, to);
    } else {
        toggle_piece(board, from_piece, to);
    }

    board->halfmove_clock++;
    board->turn = -board->turn;
    if (board->turn == WHITE) {
        board->fullmove_counter++;
    }

    push_irreversible_vector(board->irreversible_moves, irreversible);
}

void unmake_move(Board *board, uint16_t move) {
    uint16_t from = move & 0b111111;
    uint16_t to = (move >> 6) & 0b111111;
    uint16_t flags = (move >> 12) & 0b1111;

    board->turn = -board->turn;
    if (board->turn == BLACK) {
        board->fullmove_counter--;
    }

    Irreversible irreversible =
        pop_back_irreversible_vector(board->irreversible_moves);
    board->en_passant = irreversible.en_passant;
    board->castling = irreversible.castling;
    board->halfmove_clock = irreversible.halfmove_clock;

    Piece from_piece = get_piece(board, to);

    if (en_passant(flags)) {
        toggle_piece(board, irreversible.captured_piece,
                     board->en_passant - board->turn);
    } else if (capture(flags)) {
        toggle_piece(board, irreversible.captured_piece, to);
    }

    handle_castle(board, flags);

    toggle_piece(board, from_piece, to);
    if (promotion(flags)) {
        from_piece.type = PAWN;
    }
    toggle_piece(board, from_piece, from);
}
