#include "queen.h"

#include "ray.h"

#include "../board.h"
#include "../move.h"
#include <stdio.h>

uint64_t queen_attacks(uint64_t queens, uint64_t occupied) {
    uint64_t attacks = 0;
    while (queens) {
        size_t square = __builtin_ctzll(queens);
        // NORTH, EAST, SOUTH, WEST, NORTH_EAST, NORTH_WEST, SOUTH_EAST,
        // SOUTH_WEST
        for (enum Direction dir = 0; dir < 8; dir++) {
            uint64_t ray_attacks;
            if (dir % 4 < 2) {
                ray_attacks = get_positive_ray_attacks(occupied, square, dir);
            } else {
                ray_attacks = get_negative_ray_attacks(occupied, square, dir);
            }
            attacks |= ray_attacks;
        }
        queens ^= (uint64_t)1 << square;
    }
    return attacks;
}

uint64_t xray_queen_attacks(uint64_t queen, uint64_t occupied,
                            uint64_t blockers) {
    uint64_t attacks = queen_attacks(queen, occupied);
    blockers &= attacks;
    return attacks ^ queen_attacks(queen, occupied ^ blockers);
}

MoveVector *generate_queen_moves(Board *board) {
    MoveVector *moves = new_move_vector();
    uint64_t own_pieces =
        board->turn == WHITE ? board->white_pieces : board->black_pieces;
    uint64_t enemy_pieces =
        board->turn == WHITE ? board->black_pieces : board->white_pieces;
    uint64_t occupied = own_pieces | enemy_pieces;
    uint64_t queens = board->queens & own_pieces;
    while (queens) {
        size_t square = __builtin_ctzll(queens);
        uint64_t attacks = queen_attacks((uint64_t)1 << square, occupied);
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
        queens ^= (uint64_t)1 << square;
    }

    return moves;
}
