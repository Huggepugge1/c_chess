#include "move_generation/king.h"
#include "move_generation/knight.h"
#include "move_generation/ray.h"
#include "uci.h"

int main() {
    generate_knight_attack_bitboards();
    generate_king_attack_bitboards();
    generate_ray_attack_bitboards();
    run_uci();
    return 0;
}
