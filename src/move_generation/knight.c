#include <stddef.h>
#include <stdint.h>

#include "../board.h"
#include "../move.h"
#include "knight.h"

#define NOT_AFILE 0xfefefefefefefefe;
#define NOT_ABFILE 0xfcfcfcfcfcfcfcfc;
#define NOT_HFILE 0x7f7f7f7f7f7f7f7f;
#define NOT_GHFILE 0x3f3f3f3f3f3f3f3f;

static uint64_t KNIGHT_ATTACK_BITBOARDS[64];

uint64_t knight_attacks(uint64_t knights) {
    uint64_t l1 = (knights >> 1) & NOT_HFILE;
    uint64_t l2 = (knights >> 2) & NOT_GHFILE;
    uint64_t r1 = (knights << 1) & NOT_AFILE;
    uint64_t r2 = (knights << 2) & NOT_ABFILE;
    uint64_t h1 = l1 | r1;
    uint64_t h2 = l2 | r2;
    return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
}

void generate_knight_attack_bitboards() {
    for (size_t square = 0; square < 64; square++) {
        KNIGHT_ATTACK_BITBOARDS[square] = knight_attacks((uint64_t)1 << square);
    }
}

MoveVector *generate_knight_moves(Board *board) {
    MoveVector *moves = new_move_vector();
    uint64_t own_pieces =
        board->turn == WHITE ? board->white_pieces : board->black_pieces;
    uint64_t enemy_pieces =
        board->turn == WHITE ? board->black_pieces : board->white_pieces;
    uint64_t knights = board->knights & own_pieces;
    while (knights) {
        size_t square = __builtin_ctzll(knights);
        uint64_t attacks = KNIGHT_ATTACK_BITBOARDS[square] & ~own_pieces;
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
        knights ^= (uint64_t)1 << square;
    }

    return moves;
}
