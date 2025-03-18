#include "generate.h"
#include "../vector.h"
#include "bishop.h"
#include "king.h"
#include "knight.h"
#include "pawn.h"
#include "queen.h"
#include "ray.h"
#include "rook.h"
#include <stdio.h>

uint64_t calculate_attack_bitboard(Board *board) {
    uint64_t own_pieces =
        board->turn == WHITE ? board->black_pieces : board->white_pieces;
    uint64_t enemy_pieces =
        board->turn == WHITE ? board->white_pieces : board->black_pieces;
    uint64_t occupied = own_pieces | enemy_pieces;

    uint64_t attacks = 0;

    if (board->turn == WHITE) {
        attacks = pawn_attacks(board->pawns & own_pieces, BLACK);
    } else {
        attacks = pawn_attacks(board->pawns & own_pieces, WHITE);
    }
    attacks |= rook_attacks(board->rooks & own_pieces, occupied);
    attacks |= knight_attacks(board->knights & own_pieces);
    attacks |= bishop_attacks(board->bishops & own_pieces, occupied);
    attacks |= queen_attacks(board->queens & own_pieces, occupied);
    return attacks;
}

uint64_t calculate_checkers(Board *board) {
    uint64_t own_pieces =
        board->turn == WHITE ? board->black_pieces : board->white_pieces;
    uint64_t enemy_pieces =
        board->turn == WHITE ? board->white_pieces : board->black_pieces;
    uint64_t occupied = own_pieces | enemy_pieces;
    uint64_t king = board->kings & own_pieces;

    uint64_t checkers;

    checkers = pawn_attacks(king, board->turn) & enemy_pieces & board->pawns;
    checkers |= rook_attacks(king, occupied) & enemy_pieces & board->rooks;
    checkers |= knight_attacks(king) & enemy_pieces & board->knights;
    checkers |= bishop_attacks(king, occupied) & enemy_pieces & board->bishops;
    checkers |= queen_attacks(king, occupied) & enemy_pieces & board->queens;
    return checkers;
}

uint64_t get_ray(size_t square1, size_t square2) {
    uint64_t ray = 0;
    while (square1 != square2) {
        if (square1 % 8 < square2 % 8) {
            square1++;
        } else if (square1 % 8 > square2 % 8) {
            square1--;
        }
        if (square1 / 8 < square2 / 8) {
            square1 += 8;
        } else if (square1 / 8 > square2 / 8) {
            square1 -= 8;
        }
        ray |= (uint64_t)1 << square1;
    }
    return ray;
}

void calculate_pinned_pieces(Board *board) {
    uint64_t own_pieces =
        board->turn == WHITE ? board->white_pieces : board->black_pieces;
    uint64_t enemy_pieces =
        board->turn == WHITE ? board->black_pieces : board->white_pieces;
    uint64_t occupied = own_pieces | enemy_pieces;

    uint64_t king = board->kings & own_pieces;
    size_t king_square = __builtin_ctzll(king);

    board->pinned_pieces = 0;

    uint64_t pinner = xray_rook_attacks(occupied, own_pieces, king) &
                      enemy_pieces & board->bishops;
    board->pinner = pinner;
    while (pinner) {
        size_t square = __builtin_ctzll(pinner);
        board->pinned_pieces |= get_ray(king_square, square) & own_pieces;
        pinner &= pinner - 1;
    }
    pinner = xray_bishop_attacks(occupied, own_pieces, king) & enemy_pieces &
             board->bishops;
    board->pinner |= pinner;
    while (pinner) {
        size_t square = __builtin_ctzll(pinner);
        board->pinned_pieces |= get_ray(king_square, square) & own_pieces;
        pinner &= pinner - 1;
    }
    pinner = xray_queen_attacks(occupied, own_pieces, king) & enemy_pieces &
             board->queens;
    board->pinner |= pinner;
    while (pinner) {
        size_t square = __builtin_ctzll(pinner);
        board->pinned_pieces |= get_ray(king_square, square) & own_pieces;
        pinner &= pinner - 1;
    }
}

uint64_t calculate_pinned_ray(Board *board) {
    uint64_t own_pieces =
        board->turn == WHITE ? board->white_pieces : board->black_pieces;
    uint64_t enemy_pieces =
        board->turn == WHITE ? board->black_pieces : board->white_pieces;
    uint64_t king = board->kings & own_pieces;
    if (__builtin_popcountll(board->pinner) != 1) {
        return 0;
    }

    size_t king_square = __builtin_ctzll(king);
    size_t pinner_square = __builtin_ctzll(board->pinner);

    return get_ray(king_square, pinner_square);
}

MoveVector *generate_moves(Board *board) {
    board->attacks = calculate_attack_bitboard(board);
    board->checkers = calculate_checkers(board);
    calculate_pinned_pieces(board);
    board->pinned_ray = calculate_pinned_ray(board);

    if (__builtin_popcount(board->checkers) == 2) {
        return generate_king_moves(board);
    }

    MoveVector *moves = new_move_vector();

    MoveVector *pawn_moves = generate_pawn_moves(board);
    append_move_vector(moves, pawn_moves);
    free_move_vector(pawn_moves);

    MoveVector *rook_moves = generate_rook_moves(board);
    append_move_vector(moves, rook_moves);
    free_move_vector(rook_moves);

    MoveVector *knight_moves = generate_knight_moves(board);
    append_move_vector(moves, knight_moves);
    free_move_vector(knight_moves);

    MoveVector *bishop_moves = generate_bishop_moves(board);
    append_move_vector(moves, bishop_moves);
    free_move_vector(bishop_moves);

    MoveVector *queen_moves = generate_queen_moves(board);
    append_move_vector(moves, queen_moves);
    free_move_vector(queen_moves);

    MoveVector *king_moves = generate_king_moves(board);
    append_move_vector(moves, king_moves);
    free_move_vector(king_moves);

    return moves;
}
