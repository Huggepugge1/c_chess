#include "uci.h"
#include "board.h"
#include "chess_engine.h"
#include "move.h"
#include "perft.h"

#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_t running_thread;
pthread_mutex_t stopper_mutex;
bool stopper = true;

void to_lower(char *string) {
    for (size_t i = 0; i < strlen(string); i++) {
        string[i] = tolower(string[i]);
    }
}

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
        to_lower(command->strings[1]);
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
    if (command->len < 2) {
        printf("The `position` command has at least 1 argument\n");
        return;
    }
    to_lower(command->strings[1]);
    if (strcmp(command->strings[1], "startpos") == 0) {
        destroy_board(chess_engine->board);
        chess_engine->board = new_board(NULL);
    } else if (strcmp(command->strings[1], "fen") == 0) {
        if (command->len < 8) {
            printf("The `fen` subcommand has at exactly 6 argument\n");
            return;
        }
        destroy_board(chess_engine->board);
        command->strings += 2;
        command->len -= 2;
        chess_engine->board = new_board(command);
        command->strings -= 2;
        command->len += 2;
    } else {
        printf("`position` has to be followed by [startpos | fen], found: %s\n",
               command->strings[1]);
    }
}

void handle_command_export(Split *command, ChessEngine *chess_engine) {
    if (command->len == 4) {
        to_lower(command->strings[1]);
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

void handle_command_print_fen(Split *command, ChessEngine *chess_engine) {
    if (command->len == 1) {
        print_fen(chess_engine->board);
    } else {
        printf("The `printfen` command has exactly 0 arguments\n");
    }
}

void handle_command_move(Split *command, ChessEngine *chess_engine) {
    if (command->len == 2) {
        to_lower(command->strings[1]);
        uint16_t move =
            move_from_string(command->strings[1], chess_engine->board);
        make_move(chess_engine->board, move);
    }
}

void handle_command_go(Split *command, ChessEngine *chess_engine) {
    if (command->len == 3) {
        pthread_mutex_lock(&stopper_mutex);
        stopper = true;
        pthread_mutex_unlock(&stopper_mutex);
        void *status;
        pthread_join(running_thread, &status);
        to_lower(command->strings[1]);
        if (strcmp(command->strings[1], "perft") == 0) {
            size_t depth = atoi(command->strings[2]);

            PerftArgs *args = malloc(sizeof(PerftArgs));
            args->chess_engine = chess_engine;
            args->depth = depth;

            pthread_mutex_lock(&stopper_mutex);
            stopper = false;
            pthread_mutex_unlock(&stopper_mutex);

            pthread_create(&running_thread, NULL, perft, (void *)args);
        }
    } else {
        printf("The `go` command has exactly 1 arguments\n");
    }
}

void handle_command_stop(Split *command) {
    if (command->len == 1) {
        void *status;
        pthread_mutex_lock(&stopper_mutex);
        if (!stopper) {
            stopper = true;
            pthread_mutex_unlock(&stopper_mutex);
            pthread_join(running_thread, &status);
            pthread_mutex_lock(&stopper_mutex);
        }
        pthread_mutex_unlock(&stopper_mutex);
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
        to_lower(command->strings[0]);
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

        } else if (strcmp(command->strings[0], "printfen") == 0) {
            handle_command_print_fen(command, chess_engine);

        } else if (strcmp(command->strings[0], "move") == 0) {
            handle_command_move(command, chess_engine);

        } else if (strcmp(command->strings[0], "go") == 0) {
            handle_command_go(command, chess_engine);

        } else if (strcmp(command->strings[0], "stop") == 0) {
            handle_command_stop(command);

        } else if (strcmp(command->strings[0], "quit") == 0) {
            pthread_mutex_lock(&stopper_mutex);
            if (!stopper) {
                stopper = true;
                pthread_mutex_unlock(&stopper_mutex);
                void *status;
                pthread_join(running_thread, &status);
                pthread_mutex_lock(&stopper_mutex);
            }
            stopper = true;
            pthread_mutex_unlock(&stopper_mutex);
            free_chess_engine(chess_engine);
            free_split(command);
            exit(0);

        } else {
            printf("Unhandled command: %s\n", command->strings[0]);
        }
        free_split(command);
    }
}
