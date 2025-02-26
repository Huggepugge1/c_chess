#include "bishop.h"
#include "../move.h"
#include "../vector.h"
#include "ray.h"

MoveVector *generate_bishop_moves(Board *board) {
    MoveVector *moves = new_move_vector();
    uint64_t own_pieces =
        board->turn == WHITE ? board->white_pieces : board->black_pieces;
    uint64_t enemy_pieces =
        board->turn == WHITE ? board->black_pieces : board->white_pieces;
    uint64_t bishops = board->bishops & own_pieces;
    while (bishops) {
        size_t square = __builtin_ctzll(bishops);
        // NORTH_EAST, SOUTH_EAST, SOUTH_WEST, NORTH_WEST
        for (enum Direction dir = 4; dir < 8; dir++) {
            uint64_t attacks;
            if (dir < 6) {
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
        bishops ^= (uint64_t)1 << square;
    }

    return moves;
}
