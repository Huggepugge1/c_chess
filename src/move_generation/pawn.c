#include "pawn.h"
#include "../move.h"
#include "../vector.h"
#include <stdio.h>

#define NOT_A_FILE 0xfefefefefefefefe
#define NOT_H_FILE 0x7f7f7f7f7f7f7f7f

void add_pawn_moves_to_vector(size_t from_square, size_t to_square,
                              MoveVector *moves, uint64_t flags) {
    if (to_square / 8 == 7 || to_square / 8 == 0) {
        uint64_t promotion_move =
            new_move(from_square, to_square, flags | 0b1000);
        push_move_vector(moves, promotion_move);
        promotion_move = new_move(from_square, to_square, flags | 0b1001);
        push_move_vector(moves, promotion_move);
        promotion_move = new_move(from_square, to_square, flags | 0b1010);
        push_move_vector(moves, promotion_move);
        promotion_move = new_move(from_square, to_square, flags | 0b1011);
        push_move_vector(moves, promotion_move);
    } else {
        uint64_t move = new_move(from_square, to_square, flags);
        push_move_vector(moves, move);
    }
}

MoveVector *generate_white_pawn_moves(Board *board) {
    MoveVector *moves = new_move_vector();
    uint64_t pawns = board->pawns & board->white_pieces;
    uint64_t occupied = board->white_pieces | board->black_pieces;
    while (pawns) {
        size_t square = __builtin_ctzll(pawns);
        if (!(occupied & (uint64_t)1 << (square + WHITE))) {
            add_pawn_moves_to_vector(square, square + WHITE, moves, QUIET_MOVE);
            if (square / 8 == 1 &&
                !(occupied & (uint64_t)1 << (square + 2 * WHITE))) {
                add_pawn_moves_to_vector(square, square + 2 * WHITE, moves,
                                         DOUBLE_PAWN_PUSH);
            }
        }

        if (board->black_pieces & NOT_H_FILE & ((uint64_t)1 << (square + 7))) {
            add_pawn_moves_to_vector(square, square + 7, moves, CAPTURE);
        }
        if (board->black_pieces & NOT_A_FILE & ((uint64_t)1 << (square + 9))) {
            add_pawn_moves_to_vector(square, square + 9, moves, CAPTURE);
        }
        if (board->en_passant != 64 &&
            ((square + 7 == board->en_passant && square != 47) ||
             (square + 9 == board->en_passant && square != 31))) {
            add_pawn_moves_to_vector(square, board->en_passant, moves,
                                     EN_PASSANT_CAPTURE);
        }
        pawns ^= (uint64_t)1 << square;
    }

    return moves;
}

MoveVector *generate_black_pawn_moves(Board *board) {
    MoveVector *moves = new_move_vector();
    uint64_t pawns = board->pawns & board->black_pieces;
    uint64_t occupied = board->white_pieces | board->black_pieces;
    while (pawns) {
        size_t square = __builtin_ctzll(pawns);
        if (!(occupied & (uint64_t)1 << (square + BLACK))) {
            add_pawn_moves_to_vector(square, square + BLACK, moves, QUIET_MOVE);
            if (square / 8 == 6 &&
                !(occupied & (uint64_t)1 << (square + 2 * BLACK))) {
                add_pawn_moves_to_vector(square, square + 2 * BLACK, moves,
                                         DOUBLE_PAWN_PUSH);
            }
        }

        if (board->white_pieces & NOT_H_FILE & ((uint64_t)1 << (square - 9))) {
            add_pawn_moves_to_vector(square, square - 9, moves, CAPTURE);
        }

        if (board->white_pieces & NOT_A_FILE & ((uint64_t)1 << (square - 7))) {
            add_pawn_moves_to_vector(square, square - 7, moves, CAPTURE);
        }
        if (board->en_passant != 64 &&
            ((square - 9 == board->en_passant && square != 32) ||
             (square - 7 == board->en_passant && square != 23))) {
            add_pawn_moves_to_vector(square, board->en_passant, moves,
                                     EN_PASSANT_CAPTURE);
        }
        pawns ^= (uint64_t)1 << square;
    }

    return moves;
}

MoveVector *generate_pawn_moves(Board *board) {
    if (board->turn == WHITE) {
        return generate_white_pawn_moves(board);
    } else {
        return generate_black_pawn_moves(board);
    }
}
