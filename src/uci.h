#pragma once

#include <stdlib.h>

typedef struct Split Split;

struct Split {
    char **strings;
    size_t len;
};

/**
 * Run the main uci loop
 */
void run_uci();

/**
 * Split the command string into tokens.
 *
 * @param command The string to split
 *
 * @returns Pointer to the array containing the split string
 */
Split *split(char *command);

/**
 * Free the array containing a split string
 *
 * @param char array The array
 * @param len The length of the array
 */
void free_split(Split *split);
