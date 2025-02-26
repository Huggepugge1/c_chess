#include "exitcode.h"

#include <stdio.h>
#include <stdlib.h>

void crash(enum ExitCode exitcode) {
    switch (exitcode) {
    case EXITCODE_CHESS_ENGINE_ALLOCATION_FAILED:
        perror("Chess Engine allocation failed\n");
        break;
    case EXITCODE_BOARD_ALLOCATION_FAILED:
        perror("Board allocation failed\n");
        break;
    case EXITCODE_VECTOR_ALLOCATION_FAILED:
        perror("Vector allocation failed\n");
        break;
    case EXITCODE_VECTOR_REALLOCATION_FAILED:
        perror("Vector reallocation failed\n");
        break;

    case EXITCODE_EMPTY_PIECE_MODIFIED:
        perror("Tried to modify board with an empty piece\n");
        break;
    case EXITCODE_PIECE_WITHOUT_VALID_TYPE:
        perror("Piece had no valid type\n");
        break;
    case EXITCODE_PIECE_WITHOUT_VALID_COLOR:
        perror("Piece had no valid color\n");
        break;

    case EXITCODE_VECTOR_OUT_OF_BOUNDS:
        perror("Vector out of bounds\n");
        break;

    case EXITCODE_UNREACHABLE:
        perror("Reached unreachable code\n");
        break;
    }
    exit(exitcode);
}
