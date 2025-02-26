#include <stddef.h>
#include <stdint.h>

enum Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST,
    NORTH_WEST,
    NORTH_EAST,
    SOUTH_EAST,
    SOUTH_WEST,
};

/**
 * Precompute all possible ray attacks
 */
void generate_ray_attack_bitboards();

/**
 * Get the positive ray attacks
 *
 * @param occupied The occupied squares
 * @param square The square to get the attacks for
 * @param direction The direction to get the attacks in
 *
 * @returns The attacks
 */
uint64_t get_positive_ray_attacks(uint64_t occupied, size_t square,
                                  enum Direction direction);

/**
 * Get the negative ray attacks
 *
 * @param occupied The occupied squares
 * @param square The square to get the attacks for
 * @param direction The direction to get the attacks in
 *
 * @returns The attacks
 */
uint64_t get_negative_ray_attacks(uint64_t occupied, size_t square,
                                  enum Direction direction);
