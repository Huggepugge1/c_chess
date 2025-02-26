#include "../board.h"
#include "../move.h"

#include <stddef.h>
#include <stdint.h>

#define NOT_AFILE 0x7f7f7f7f7f7f7f7f
#define NOT_HFILE 0xfefefefefefefefe

#define north_one(bitboard) (bitboard << 8)
#define south_one(bitboard) (bitboard >> 8)
#define east_one(bitboard) ((bitboard >> 1) & NOT_AFILE)
#define west_one(bitboard) ((bitboard << 1) & NOT_HFILE)

static uint64_t KING_ATTACK_BITBOARDS[64];

uint64_t king_attacks(uint64_t kings) {
    uint64_t attacks = east_one(kings) | west_one(kings);
    uint64_t new_kings = kings | attacks;
    return new_kings | north_one(new_kings) | south_one(new_kings);
}

void generate_king_attack_bitboards() {
    for (size_t square = 0; square < 64; square++) {
        KING_ATTACK_BITBOARDS[square] = king_attacks((uint64_t)1 << square);
    }
}

MoveVector *generate_king_moves(Board *board) {
    MoveVector *moves = new_move_vector();
    uint64_t own_pieces =
        board->turn == WHITE ? board->white_pieces : board->black_pieces;
    uint64_t enemy_pieces =
        board->turn == WHITE ? board->black_pieces : board->white_pieces;
    uint64_t kings = board->kings & own_pieces;
    while (kings) {
        size_t square = __builtin_ctzll(kings);
        uint64_t attacks = KING_ATTACK_BITBOARDS[square] & ~own_pieces;
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
        kings ^= (uint64_t)1 << square;
    }

    return moves;
}
