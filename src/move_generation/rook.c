#include "rook.h"
#include "../board.h"
#include "../move.h"
#include "../vector.h"
#include "ray.h"

#include <stdio.h>

uint64_t rook_attacks(uint64_t rooks, uint64_t occupied) {
    uint64_t attacks = 0;
    while (rooks) {
        size_t square = __builtin_ctzll(rooks);
        // NORTH, EAST, SOUTH, WEST
        for (enum Direction dir = 0; dir < 4; dir++) {
            uint64_t ray_attacks;
            if (dir < 2) {
                ray_attacks = get_positive_ray_attacks(occupied, square, dir);
            } else {
                ray_attacks = get_negative_ray_attacks(occupied, square, dir);
            }
            attacks |= ray_attacks;
        }
        rooks ^= (uint64_t)1 << square;
    }
    return attacks;
}

uint64_t xray_rook_attacks(uint64_t occupied, uint64_t blockers,
                           uint64_t rook) {
    uint64_t attacks = rook_attacks(occupied, rook);
    blockers &= attacks;
    return attacks ^ rook_attacks(occupied ^ blockers, rook);
}

MoveVector *generate_rook_moves(Board *board) {
    MoveVector *moves = new_move_vector();
    uint64_t own_pieces =
        board->turn == WHITE ? board->white_pieces : board->black_pieces;
    uint64_t enemy_pieces =
        board->turn == WHITE ? board->black_pieces : board->white_pieces;
    uint64_t occupied = own_pieces | enemy_pieces;
    uint64_t rooks = board->rooks & own_pieces;
    while (rooks) {
        size_t square = __builtin_ctzll(rooks);
        uint64_t attacks = rook_attacks((uint64_t)1 << square, occupied);
        attacks &= ~own_pieces;
        while (attacks) {
            size_t to = __builtin_ctzll(attacks);
            if (enemy_pieces & ((uint64_t)1 << to)) {
                uint64_t move = new_move(square, to, CAPTURE);
                push_move_vector(moves, move);
            } else {
                uint64_t move = new_move(square, to, QUIET_MOVE);
                push_move_vector(moves, move);
            }
            attacks &= attacks - 1;
        }
        rooks ^= (uint64_t)1 << square;
    }

    return moves;
}
