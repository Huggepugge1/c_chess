#include "uci.h"
#include "board.h"
#include "chess_engine.h"
#include "move.h"

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Split Split;

bool running = false;

void handle_command_uci(Split *command) {
    if (command->len == 1) {
        printf("id name c_chess\n");
        printf("id author Hugo Lindström\n");
        printf("uciok\n");
    } else {
        printf("The `uci` command has exactly 1 argument\n");
    }
}

void handle_command_debug(Split *command, ChessEngine *chess_engine) {
    if (command->len == 2) {
        if (strcmp(command->strings[1], "on") == 0) {
            chess_engine->debug = true;
            printf("Turned on debug mode\n");
        } else if (strcmp(command->strings[1], "off") == 0) {
            chess_engine->debug = false;
            printf("Turned off debug mode\n");
        } else {
            printf("`debug` has to be followed by [on | off], found: %s\n",
                   command->strings[1]);
        }
    } else {
        printf("The `debug` command has exactly 1 argument\n");
    }
}

void handle_command_isready(Split *command) {
    if (command->len == 1) {
        printf("readyok\n");
    } else {
        printf("The `isready` command has exactly 0 arguments\n");
    }
}

void handle_command_position(Split *command, ChessEngine *chess_engine) {
    if (strcmp(command->strings[1], "startpos") == 0) {
        chess_engine->board = new_board(NULL);
    } else if (strcmp(command->strings[1], "fen") == 0) {
        command->strings += 2;
        command->len -= 2;
        free(chess_engine->board);
        chess_engine->board = new_board(command);
        command->strings -= 2;
        command->len += 2;
    } else {
        printf("`debug` has to be followed by [startpos | fen], found: %s\n",
               command->strings[1]);
    }
}

void handle_command_export(Split *command, ChessEngine *chess_engine) {
    if (command->len == 4) {
        enum BoardExportFormat export_format;
        if (strcmp(command->strings[1], "c") == 0) {
            export_format = C;
        } else if (strcmp(command->strings[1], "fen") == 0) {
            export_format = FEN;
        } else {
            printf("`export` has to be followed by [c | fen], found: %s\n",
                   command->strings[1]);
        }
        export_board(chess_engine->board, export_format, command->strings[2],
                     command->strings[3]);
        printf("Exported the position to `%s`: `%s`\n", command->strings[3],
               command->strings[2]);
    } else {
        printf("The `export` command has exactly 2 arguments\n");
    }
}

void handle_command_move(Split *command, ChessEngine *chess_engine) {
    if (command->len == 2) {
        print_move(move_from_string(command->strings[1], chess_engine->board));
    }
}

void handle_command_stop(Split *command) {
    if (command->len == 1) {
        running = false;
    } else {
        printf("The `stop` command has exactly 0 arguments\n");
    }
}

size_t get_total_words(char *command) {
    size_t result = 1;
    char current = *command;
    while (*command) {
        if (current != ' ' && *command == ' ') {
            result++;
            while (*command && *command == ' ') {
                command++;
            }
            current = *command;
        } else {
            command++;
        }
    }
    if (current == ' ' || current == '\0') {
        return 0;
    }
    return result;
}

Split *split(char *string) {
    Split *result = malloc(sizeof(Split));
    result->len = get_total_words(string);
    result->strings = calloc(result->len, sizeof(char *));

    char *current = string;
    size_t i = 0;

    while (*string) {
        if (*current != ' ' && *string == ' ') {
            char save = *string;
            *string = '\0';
            result->strings[i++] = strdup(current);
            *string = save;
            while (*string && *string == ' ') {
                string++;
            }
            current = string;
        } else {
            string++;
        }
    }

    if (*current && *current != ' ') {
        result->strings[i] = strdup(current);
    }

    return result;
}

void free_split(Split *split) {
    for (size_t i = 0; i < split->len; i++) {
        free(split->strings[i]);
    }
    free(split->strings);
    free(split);
}

void trim(char *buf, size_t len) {
    for (int i = len - 1; i >= 0; i--) {
        if (buf[i] == '\n' || buf[i] == ' ' || buf[i] == '\0' ||
            buf[i] == '\t') {
            buf[i] = 0;
        } else {
            break;
        }
    }
}

void run_uci() {
    ChessEngine *chess_engine = new_chess_engine();
    char buf[1000];
    while (1) {
        memset(buf, 0, 1000);
        fgets(buf, 1000, stdin);
        trim(buf, 1000);
        Split *command = split(buf);
        if (!command->len) {
            free_split(command);
            continue;
        }
        if (strcmp(command->strings[0], "uci") == 0) {
            handle_command_uci(command);

        } else if (strcmp(command->strings[0], "debug") == 0) {
            handle_command_debug(command, chess_engine);

        } else if (strcmp(command->strings[0], "isready") == 0) {
            handle_command_isready(command);

        } else if (strcmp(command->strings[0], "position") == 0) {
            handle_command_position(command, chess_engine);

        } else if (strcmp(command->strings[0], "printboard") == 0) {
            print_board(chess_engine->board);

        } else if (strcmp(command->strings[0], "export") == 0) {
            handle_command_export(command, chess_engine);

        } else if (strcmp(command->strings[0], "move") == 0) {
            handle_command_move(command, chess_engine);

        } else if (strcmp(command->strings[0], "stop") == 0) {
            handle_command_stop(command);

        } else if (strcmp(command->strings[0], "quit") == 0) {
            free_chess_engine(chess_engine);
            free_split(command);
            exit(0);

        } else {
            printf("Unhandled command: %s\n", command->strings[0]);
        }
        free_split(command);
    }
}
