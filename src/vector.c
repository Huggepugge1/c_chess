#include "vector.h"
#include "exitcode.h"

#include <stdio.h>
#include <stdlib.h>

IrreversibleVector *new_irreversible_vector() {
    IrreversibleVector *vector = malloc(sizeof(IrreversibleVector));
    vector->capacity = 256;
    vector->size = 0;
    vector->data = malloc(sizeof(Irreversible) * vector->capacity);
    return vector;
}

void insert_irreversible_vector(IrreversibleVector *vector,
                                Irreversible element, size_t index) {
    if (index > vector->size) {
        crash(EXITCODE_VECTOR_OUT_OF_BOUNDS);
    }
    if (vector->size == vector->capacity) {
        vector->capacity *= 2;
        vector->data =
            realloc(vector->data, sizeof(Irreversible) * vector->capacity);
    }
    for (size_t i = vector->size; i > index; i--) {
        vector->data[i] = vector->data[i - 1];
    }
    vector->data[index] = element;
    vector->size++;
}

void push_irreversible_vector(IrreversibleVector *vector,
                              Irreversible element) {
    if (vector->size == vector->capacity) {
        vector->capacity *= 2;
        vector->data =
            realloc(vector->data, sizeof(Irreversible) * vector->capacity);
    }
    vector->data[vector->size++] = element;
}

Irreversible get_irreversible_vector(IrreversibleVector *vector, size_t index) {
    if (index >= vector->size) {
        crash(EXITCODE_VECTOR_OUT_OF_BOUNDS);
    }
    return vector->data[index];
}

Irreversible pop_irreversible_vector(IrreversibleVector *vector, size_t index) {
    if (index >= vector->size) {
        crash(EXITCODE_VECTOR_OUT_OF_BOUNDS);
    }
    Irreversible element = vector->data[index];
    for (size_t i = index; i < vector->size - 1; i++) {
        vector->data[i] = vector->data[i + 1];
    }
    vector->size--;
    return element;
}

Irreversible pop_back_irreversible_vector(IrreversibleVector *vector) {
    if (vector->size == 0) {
        crash(EXITCODE_VECTOR_OUT_OF_BOUNDS);
    }
    return vector->data[--vector->size];
}

void clear_irreversible_vector(IrreversibleVector *vector) {
    vector->size = 0;
}

void free_irreversible_vector(IrreversibleVector *vector) {
    free(vector->data);
    free(vector);
}

MoveVector *new_move_vector() {
    MoveVector *vector = malloc(sizeof(MoveVector));
    vector->capacity = 1;
    vector->size = 0;
    vector->data = malloc(sizeof(uint16_t) * vector->capacity);
    return vector;
}

void push_move_vector(MoveVector *vector, uint16_t element) {
    if (vector->size == vector->capacity) {
        vector->capacity *= 2;
        vector->data =
            realloc(vector->data, sizeof(uint16_t) * vector->capacity);
    }
    vector->data[vector->size++] = element;
}

void append_move_vector(MoveVector *vector, MoveVector *other) {
    for (size_t i = 0; i < other->size; i++) {
        push_move_vector(vector, other->data[i]);
    }
}

uint16_t get_move_vector(MoveVector *vector, size_t index) {
    if (index >= vector->size) {
        crash(EXITCODE_VECTOR_OUT_OF_BOUNDS);
    }
    return vector->data[index];
}

uint16_t pop_move_vector(MoveVector *vector, size_t index) {
    if (index >= vector->size) {
        crash(EXITCODE_VECTOR_OUT_OF_BOUNDS);
    }
    uint16_t element = vector->data[index];
    for (size_t i = index; i < vector->size - 1; i++) {
        vector->data[i] = vector->data[i + 1];
    }
    vector->size--;
    return element;
}

uint16_t pop_back_move_vector(MoveVector *vector) {
    if (vector->size == 0) {
        crash(EXITCODE_VECTOR_OUT_OF_BOUNDS);
    }
    return vector->data[--vector->size];
}

void clear_move_vector(MoveVector *vector) {
    vector->size = 0;
}

void free_move_vector(MoveVector *vector) {
    free(vector->data);
    free(vector);
}
