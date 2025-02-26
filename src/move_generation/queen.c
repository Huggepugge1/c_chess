#include "queen.h"

#include "ray.h"

#include "../board.h"
#include "../move.h"

MoveVector *generate_queen_moves(Board *board) {
    MoveVector *moves = new_move_vector();
    uint64_t own_pieces =
        board->turn == WHITE ? board->white_pieces : board->black_pieces;
    uint64_t enemy_pieces =
        board->turn == WHITE ? board->black_pieces : board->white_pieces;
    uint64_t queens = board->queens & own_pieces;
    while (queens) {
        size_t square = __builtin_ctzll(queens);
        // NORTH, EAST, SOUTH, WEST, NORTH_EAST, NORTH_WEST, SOUTH_EAST,
        // SOUTH_WEST
        for (enum Direction dir = 0; dir < 8; dir++) {
            uint64_t attacks;
            if (dir % 4 < 2) {
                attacks = get_positive_ray_attacks(own_pieces | enemy_pieces,
                                                   square, dir);
            } else {
                attacks = get_negative_ray_attacks(own_pieces | enemy_pieces,
                                                   square, dir);
            }
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
        }
        queens ^= (uint64_t)1 << square;
    }

    return moves;
}
