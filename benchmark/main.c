#include "../src/chess_engine.h"
#include "../src/move_generation/king.h"
#include "../src/move_generation/knight.h"
#include "../src/move_generation/ray.h"
#include "../src/perft.h"

#include <pthread.h>

extern bool stopper;

int main() {
    generate_knight_attack_bitboards();
    generate_king_attack_bitboards();
    generate_ray_attack_bitboards();

    ChessEngine *chess_engine = new_chess_engine();
    PerftArgs *args = malloc(sizeof(PerftArgs));

    stopper = false;

    args->chess_engine = chess_engine;
    args->depth = 6;
    pthread_t perft_thread;
    pthread_create(&perft_thread, NULL, perft, (void *)args);
    pthread_join(perft_thread, NULL);
    return 0;
}
