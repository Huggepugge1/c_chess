#include "perft.h"
#include "chess_engine.h"
#include "move.h"
#include "move_generation/generate.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern pthread_mutex_t stopper_mutex;
extern bool stopper;

size_t perft_helper(ChessEngine *chess_engine, size_t depth) {
    if (depth == 0) {
        return 1;
    }

    MoveVector *moves = generate_moves(chess_engine->board);

    size_t nodes = 0;
    for (size_t i = 0; i < moves->size; i++) {
        pthread_mutex_lock(&stopper_mutex);
        if (stopper) {
            pthread_mutex_unlock(&stopper_mutex);
            break;
        }
        pthread_mutex_unlock(&stopper_mutex);

        uint16_t move = get_move_vector(moves, i);

        make_move(chess_engine->board, move);
        nodes += perft_helper(chess_engine, depth - 1);
        unmake_move(chess_engine->board, move);
    }
    sched_yield();
    free_move_vector(moves);
    return nodes;
}

void *perft(void *args) {
    PerftArgs *perft_args = (PerftArgs *)args;
    if (perft_args->depth == 0) {
        pthread_exit((void *)0);
    }

    MoveVector *moves = generate_moves(perft_args->chess_engine->board);
    size_t nodes = 0;
    for (size_t i = 0; i < moves->size; i++) {
        pthread_mutex_lock(&stopper_mutex);
        if (stopper) {
            pthread_mutex_unlock(&stopper_mutex);
            break;
        }
        pthread_mutex_unlock(&stopper_mutex);

        uint16_t move = get_move_vector(moves, i);

        make_move(perft_args->chess_engine->board, move);
        size_t current_nodes =
            perft_helper(perft_args->chess_engine, perft_args->depth - 1);
        nodes += current_nodes;

        unmake_move(perft_args->chess_engine->board, move);

        char *move_string = move_to_string(move);
        printf("%s: %zd\n", move_string, current_nodes);

        free(move_string);
    }
    printf("Nodes searched: %zd\n", nodes);

    free_move_vector(moves);
    free(args);

    pthread_mutex_lock(&stopper_mutex);
    stopper = true;
    pthread_mutex_unlock(&stopper_mutex);
    pthread_exit((void *)nodes);
}
