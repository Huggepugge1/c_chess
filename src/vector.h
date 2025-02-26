#pragma once

#include "piece.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct Castling Castling;
typedef struct Irreversible Irreversible;

typedef struct IrreversibleVector IrreversibleVector;
typedef struct MoveVector MoveVector;

struct Castling {
    bool white_king;
    bool white_queen;
    bool black_king;
    bool black_queen;
};

struct Irreversible {
    Piece captured_piece;
    size_t halfmove_clock;
    size_t en_passant;
    Castling castling;
};

struct IrreversibleVector {
    size_t capacity;
    size_t size;
    Irreversible *data;
};

struct MoveVector {
    size_t capacity;
    size_t size;
    uint16_t *data;
};

/**
 * Create a new IrreversibleVector.
 *
 * @return The new IrreversibleVector
 */
IrreversibleVector *new_irreversible_vector();

/**
 * Push an element to the IrreversibleVector.
 *
 * @param vector The vector to push to
 * @param element The element to push
 */
void push_irreversible_vector(IrreversibleVector *vector, Irreversible element);

/**
 * Get an element from the IrreversibleVector.
 *
 * @param vector The vector to get from
 * @param index The index to get from
 *
 * @return The element at the index
 */
Irreversible get_irreversible_vector(IrreversibleVector *vector, size_t index);

/**
 * Pop an element from the IrreversibleVector.
 *
 * @param vector The vector to pop from
 * @param index The index to pop from
 */
Irreversible pop_irreversible_vector(IrreversibleVector *vector, size_t index);

/**
 * Pop the last element from the IrreversibleVector.
 *
 * @param vector The vector to pop from
 */
Irreversible pop_back_irreversible_vector(IrreversibleVector *vector);

/**
 * Clear the IrreversibleVector.
 *
 * @param vector The vector to clear
 */
void clear_irreversible_vector(IrreversibleVector *vector);

/**
 * Free the IrreversibleVector.
 * The elements are freed.
 *
 * @param vector The vector to free
 */
void free_irreversible_vector(IrreversibleVector *vector);

/**
 * Create a new MoveVector.
 *
 * @return The new MoveVector
 */
MoveVector *new_move_vector();

/**
 * Push an element to the MoveVector.
 *
 * @param vector The vector to push to
 * @param element The element to push
 */
void push_move_vector(MoveVector *vector, uint16_t element);

/**
 * Append the other MoveVector to the MoveVector.
 *
 * @param vector The vector to append to
 * @param other The vector to append
 */
void append_move_vector(MoveVector *vector, MoveVector *other);

/**
 * Get an element from the MoveVector.
 *
 * @param vector The vector to get from
 * @param index The index to get from
 *
 * @return The element at the index
 */
uint16_t get_move_vector(MoveVector *vector, size_t index);

/**
 * Pop an element from the MoveVector.
 *
 * @param vector The vector to pop from
 * @param index The index to pop from
 */
uint16_t pop_move_vector(MoveVector *vector, size_t index);

/**
 * Pop the last element from the MoveVector.
 *
 * @param vector The vector to pop from
 */
uint16_t pop_back_move_vector(MoveVector *vector);

/**
 * Clear the MoveVector.
 *
 * @param vector The vector to clear
 */
void clear_move_vector(MoveVector *vector);

/**
 * Free the MoveVector.
 *
 * @param vector The vector to free
 */
void free_move_vector(MoveVector *vector);
