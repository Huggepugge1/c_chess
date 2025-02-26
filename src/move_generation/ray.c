#include "ray.h"
#include "../board.h"

#include <stdio.h>

static uint64_t RAY_ATTACK_BITBOARDS[8][64];

void generate_north_ray_attack_bitboards() {
    for (size_t square = 0; square < 64; square++) {
        uint64_t attacks = 0;
        for (size_t i = 1; square / 8 + i < 8; i++) {
            attacks |= (uint64_t)1 << (square + 8 * i);
        }
        RAY_ATTACK_BITBOARDS[NORTH][square] = attacks;
    }
}

void generate_north_east_ray_attack_bitboards() {
    for (size_t square = 0; square < 64; square++) {
        uint64_t attacks = 0;
        for (size_t i = 1, j = 1; square / 8 + i < 8 && square % 8 + j < 8;
             i++, j++) {
            attacks |= (uint64_t)1 << (square + 8 * i + j);
        }
        RAY_ATTACK_BITBOARDS[NORTH_EAST][square] = attacks;
    }
}

void generate_east_ray_attack_bitboards() {
    for (size_t square = 0; square < 64; square++) {
        uint64_t attacks = 0;
        for (size_t i = 1; square % 8 + i < 8; i++) {
            attacks |= (uint64_t)1 << (square + i);
        }
        RAY_ATTACK_BITBOARDS[EAST][square] = attacks;
    }
}

void generate_south_east_ray_attack_bitboards() {
    for (size_t square = 0; square < 64; square++) {
        uint64_t attacks = 0;
        for (size_t i = 1, j = 1; square / 8 - i < 8 && square % 8 + j < 8;
             i++, j++) {
            attacks |= (uint64_t)1 << (square - 8 * i + j);
        }
        RAY_ATTACK_BITBOARDS[SOUTH_EAST][square] = attacks;
    }
}

void generate_south_ray_attack_bitboards() {
    for (size_t square = 0; square < 64; square++) {
        uint64_t attacks = 0;
        for (size_t i = 1; square / 8 - i < 8; i++) {
            attacks |= (uint64_t)1 << (square - 8 * i);
        }
        RAY_ATTACK_BITBOARDS[SOUTH][square] = attacks;
    }
}

void generate_south_west_ray_attack_bitboards() {
    for (size_t square = 0; square < 64; square++) {
        uint64_t attacks = 0;
        for (size_t i = 1, j = 1; square / 8 - i < 8 && square % 8 - j < 8;
             i++, j++) {
            attacks |= (uint64_t)1 << (square - 8 * i - j);
        }
        RAY_ATTACK_BITBOARDS[SOUTH_WEST][square] = attacks;
    }
}

void generate_west_ray_attack_bitboards() {
    for (size_t square = 0; square < 64; square++) {
        uint64_t attacks = 0;
        for (size_t i = 1; square % 8 - i < 8; i++) {
            attacks |= (uint64_t)1 << (square - i);
        }
        RAY_ATTACK_BITBOARDS[WEST][square] = attacks;
    }
}

void generate_north_west_ray_attack_bitboards() {
    for (size_t square = 0; square < 64; square++) {
        uint64_t attacks = 0;
        for (size_t i = 1, j = 1; square / 8 + i < 8 && square % 8 - j < 8;
             i++, j++) {
            attacks |= (uint64_t)1 << (square + 8 * i - j);
        }
        RAY_ATTACK_BITBOARDS[NORTH_WEST][square] = attacks;
    }
}

void generate_ray_attack_bitboards() {
    generate_north_ray_attack_bitboards();
    generate_north_east_ray_attack_bitboards();
    generate_east_ray_attack_bitboards();
    generate_south_east_ray_attack_bitboards();
    generate_south_ray_attack_bitboards();
    generate_south_west_ray_attack_bitboards();
    generate_west_ray_attack_bitboards();
    generate_north_west_ray_attack_bitboards();
}

uint64_t get_positive_ray_attacks(uint64_t occupied, size_t square,
                                  enum Direction direction) {
    uint64_t attacks = RAY_ATTACK_BITBOARDS[direction][square];
    uint64_t blocker = attacks & occupied;
    if (blocker) {
        square = __builtin_ctzll(blocker);
        attacks ^= RAY_ATTACK_BITBOARDS[direction][square];
    }
    return attacks;
}

uint64_t get_negative_ray_attacks(uint64_t occupied, size_t square,
                                  enum Direction direction) {
    uint64_t attacks = RAY_ATTACK_BITBOARDS[direction][square];
    uint64_t blocker = attacks & occupied;
    if (blocker) {
        square = 63 - __builtin_clzll(blocker);
        attacks ^= RAY_ATTACK_BITBOARDS[direction][square];
    }
    return attacks;
}
