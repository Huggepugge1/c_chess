#include "bishop.h"
#include "../move.h"
#include "../vector.h"
#include "ray.h"

uint64_t bishop_attacks(uint64_t bishops, uint64_t own_pieces,
                        uint64_t enemy_pieces) {
    uint64_t attacks = 0;
    while (bishops) {
        size_t square = __builtin_ctzll(bishops);
        // NORTH_EAST, SOUTH_EAST, SOUTH_WEST, NORTH_WEST
        for (enum Direction dir = 4; dir < 8; dir++) {
            uint64_t ray_attacks;
            if (dir < 6) {
                ray_attacks = get_positive_ray_attacks(
                    own_pieces | enemy_pieces, square, dir);
            } else {
                ray_attacks = get_negative_ray_attacks(
                    own_pieces | enemy_pieces, square, dir);
            }
            attacks |= ray_attacks;
        }
        bishops ^= (uint64_t)1 << square;
    }
    return attacks;
}

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
        uint64_t attacks =
            bishop_attacks((uint64_t)1 << square, own_pieces, enemy_pieces);
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
        bishops ^= (uint64_t)1 << square;
    }

    return moves;
}
