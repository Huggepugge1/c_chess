#include "board.h"
#include "exitcode.h"
#include "piece.h"
#include "uci.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *board_to_fen(Board *board);

size_t string_to_square(char *string) {
    if (strcmp(string, "-") == 0) {
        return 64;
    }
    return string[0] - 'a' + (string[1] - '1') * 8;
}

char *square_to_string(size_t square) {
    char *string = calloc(3, sizeof(char));
    if (square == 64) {
        string[0] = '-';
        return string;
    }

    string[0] = 'a' + square % 8;
    string[1] = '1' + square / 8;

    return string;
}

void read_fen_part0(Board *board, char *part) {
    size_t square = 56;
    size_t pos = 0;
    while (square < 65) {
        uint64_t bitmap = (uint64_t)1 << square;
        switch (part[pos]) {
        case 'P':
            board->pawns |= bitmap;
            board->white_pieces |= bitmap;
            break;
        case 'p':
            board->pawns |= bitmap;
            board->black_pieces |= bitmap;
            break;
        case 'R':
            board->rooks |= bitmap;
            board->white_pieces |= bitmap;
            break;
        case 'r':
            board->rooks |= bitmap;
            board->black_pieces |= bitmap;
            break;
        case 'N':
            board->knights |= bitmap;
            board->white_pieces |= bitmap;
            break;
        case 'n':
            board->knights |= bitmap;
            board->black_pieces |= bitmap;
            break;
        case 'B':
            board->bishops |= bitmap;
            board->white_pieces |= bitmap;
            break;
        case 'b':
            board->bishops |= bitmap;
            board->black_pieces |= bitmap;
            break;
        case 'Q':
            board->queens |= bitmap;
            board->white_pieces |= bitmap;
            break;
        case 'q':
            board->queens |= bitmap;
            board->black_pieces |= bitmap;
            break;
        case 'K':
            board->kings |= bitmap;
            board->white_pieces |= bitmap;
            break;
        case 'k':
            board->kings |= bitmap;
            board->black_pieces |= bitmap;
            break;
        case '/':
            square -= 16;
            pos++;
            continue;
        default:
            square += part[pos] - '1';
            break;
        }
        square++;
        pos++;
    }
}

void read_fen_part1(Board *board, char *part) {
    if (strcmp(part, "w") == 0) {
        board->turn = WHITE;
    } else {
        board->turn = BLACK;
    }
}

void read_fen_part2(Board *board, char *part) {
    while (*part) {
        switch (*part) {
        case 'K':
            board->castling.white_king = true;
            break;
        case 'k':
            board->castling.black_king = true;
            break;
        case 'Q':
            board->castling.white_queen = true;
            break;
        case 'q':
            board->castling.black_queen = true;
            break;
        default:
            return;
        }
        part++;
    }
}

void read_fen_part3(Board *board, char *part) {
    board->en_passant = string_to_square(part);
}

void read_fen_part4(Board *board, char *part) {
    board->halfmove_clock = atoi(part);
}

void read_fen_part5(Board *board, char *part) {
    board->fullmove_counter = atoi(part);
}

void read_fen(Board *board, Split *fen) {
    read_fen_part0(board, fen->strings[0]);
    read_fen_part1(board, fen->strings[1]);
    read_fen_part2(board, fen->strings[2]);
    read_fen_part3(board, fen->strings[3]);
    read_fen_part4(board, fen->strings[4]);
    read_fen_part5(board, fen->strings[5]);
}

Board *new_board(Split *fen) {
    Board *board = calloc(1, sizeof(Board));
    if (!fen) {
        board = memcpy(board, &START_POS, sizeof(Board));
        if (!board) {
            crash(EXITCODE_BOARD_ALLOCATION_FAILED);
        }
    } else {
        read_fen(board, fen);
    }
    board->irreversible_moves = new_irreversible_vector();
    return board;
}

void destroy_board(Board *board) {
    free(board->irreversible_moves->data);
    free(board->irreversible_moves);
    free(board);
}

bool board_eq(const Board *board1, const Board *board2) {
    if (board1->white_pieces != board2->white_pieces) {
        return false;
    }
    if (board1->black_pieces != board2->black_pieces) {
        return false;
    }
    if (board1->pawns != board2->pawns) {
        return false;
    }
    if (board1->rooks != board2->rooks) {
        return false;
    }
    if (board1->knights != board2->knights) {
        return false;
    }
    if (board1->bishops != board2->bishops) {
        return false;
    }
    if (board1->queens != board2->queens) {
        return false;
    }
    if (board1->kings != board2->kings) {
        return false;
    }

    if (board1->castling.white_king != board2->castling.white_king) {
        return false;
    }
    if (board1->castling.white_queen != board2->castling.white_queen) {
        return false;
    }
    if (board1->castling.black_king != board2->castling.black_king) {
        return false;
    }
    if (board1->castling.black_queen != board2->castling.black_queen) {
        return false;
    }

    if (board1->en_passant != board2->en_passant) {
        return false;
    }

    if (board1->turn != board2->turn) {
        return false;
    }

    if (board1->halfmove_clock != board2->halfmove_clock) {
        return false;
    }
    if (board1->fullmove_counter != board2->fullmove_counter) {
        return false;
    }

    return true;
}

char *bool_to_string(char *string, bool b) {
    if (b) {
        strcpy(string, "true");
    } else {
        strcpy(string, "false");
    }
    return string;
}

char *color_to_string(char *string, enum Color c) {
    if (c == WHITE) {
        strcpy(string, "WHITE");
    } else {
        strcpy(string, "BLACK");
    }
    return string;
}

void export_to_c(Board *board, char *position_name, FILE *file) {
    char *bool_string = calloc(6, sizeof(char));
    char *color_string = calloc(6, sizeof(char));
    fprintf(file, "#include \"../src/board.h\"\n");
    fprintf(file, "static const Board %s = {\n", position_name);
    fprintf(file, "\t0x%016lX, // White Pieces\n", board->white_pieces);
    fprintf(file, "\t0x%016lX, // Black Pieces\n", board->black_pieces);
    fprintf(file, "\n");
    fprintf(file, "\t0x%016lX, // Pawns\n", board->pawns);
    fprintf(file, "\t0x%016lX, // Rooks\n", board->rooks);
    fprintf(file, "\t0x%016lX, // Knights\n", board->knights);
    fprintf(file, "\t0x%016lX, // Bishops\n", board->bishops);
    fprintf(file, "\t0x%016lX, // Queens\n", board->queens);
    fprintf(file, "\t0x%016lX, // Kings \n", board->kings);
    fprintf(file, "\n");
    fprintf(file, "\t{\n");
    fprintf(file, "\t\t// Castling\n");
    fprintf(file, "\t\t%s, // White King side\n",
            bool_to_string(bool_string, board->castling.white_king));
    fprintf(file, "\t\t%s, // White Queen side\n",
            bool_to_string(bool_string, board->castling.white_queen));
    fprintf(file, "\t\t%s, // Black King side\n",
            bool_to_string(bool_string, board->castling.black_king));
    fprintf(file, "\t\t%s, // Black Queen side\n",
            bool_to_string(bool_string, board->castling.black_queen));
    fprintf(file, "\t},\n");
    fprintf(file, "\n");
    fprintf(file, "\t%zd, // En Passant\t", board->en_passant);
    fprintf(file, "\n");
    fprintf(file, "\t%s, // Turn\n",
            color_to_string(color_string, board->turn));
    fprintf(file, "\t%zd, // Halfmove Clock\n", board->halfmove_clock);
    fprintf(file, "\t%zd, // Fullmove Number\n", board->fullmove_counter);
    fprintf(file, "};\n\n");

    free(bool_string);
    free(color_string);
}

char *board_to_fen(Board *board) {
    char *fen = calloc(256, sizeof(char));
    size_t pos = 0;
    size_t empty_counter = 0;
    for (size_t square = 56; square < 65; square++) {
        Piece piece = get_piece(board, square);
        if (piece.type == NONE) {
            empty_counter++;
        } else {
            if (empty_counter) {
                fen[pos++] = empty_counter + '0';
                empty_counter = 0;
            }
            fen[pos++] = piece_to_char(piece);
        }
        if ((square % 8) == 7) {
            if (empty_counter) {
                fen[pos++] = empty_counter + '0';
                empty_counter = 0;
            }
            if (square > 7) {
                fen[pos++] = '/';
            }
            square -= 16;
        }
    }

    fen[pos++] = ' ';
    if (board->turn == WHITE) {
        fen[pos++] = 'w';
    } else {
        fen[pos++] = 'b';
    }
    fen[pos++] = ' ';

    if (board->castling.white_king || board->castling.white_queen ||
        board->castling.black_king || board->castling.black_queen) {
        if (board->castling.white_king) {
            fen[pos++] = 'K';
        }
        if (board->castling.white_queen) {
            fen[pos++] = 'Q';
        }
        if (board->castling.black_king) {
            fen[pos++] = 'k';
        }
        if (board->castling.black_queen) {
            fen[pos++] = 'q';
        }
    } else {
        fen[pos++] = '-';
    }

    fen[pos++] = ' ';

    char *square_string = square_to_string(board->en_passant);
    strcpy(fen + pos, square_string);
    pos += strlen(square_string);
    free(square_string);

    fen[pos++] = ' ';
    char int_string[10];
    sprintf(int_string, "%zd", board->halfmove_clock);
    strcpy(fen + pos, int_string);
    pos += strlen(int_string);
    fen[pos++] = ' ';
    sprintf(int_string, "%zd", board->fullmove_counter);
    strcpy(fen + pos, int_string);

    return fen;
}

void export_to_fen(Board *board, FILE *file) {
    char *fen = board_to_fen(board);
    fprintf(file, "%s\n", fen);
    free(fen);
}

void export_board(Board *board, enum BoardExportFormat export_format,
                  char *position_name, char *file_name) {
    FILE *file = fopen(file_name, "a");
    if (export_format == C) {
        export_to_c(board, position_name, file);
    } else if (export_format == FEN) {
        export_to_fen(board, file);
    }
    fclose(file);
}

Piece get_piece(Board *board, uint64_t square) {
    enum Type type = NONE;
    if ((board->pawns >> square) & 1) {
        type = PAWN;
    } else if ((board->rooks >> square) & 1) {
        type = ROOK;
    } else if ((board->knights >> square) & 1) {
        type = KNIGHT;
    } else if ((board->bishops >> square) & 1) {
        type = BISHOP;
    } else if ((board->queens >> square) & 1) {
        type = QUEEN;
    } else if ((board->kings >> square) & 1) {
        type = KING;
    }

    enum Color color = WHITE;
    if ((board->black_pieces >> square) & 1) {
        color = BLACK;
    }

    return new_piece(type, color);
}

void print_bitmap(uint64_t bitmap) {
    for (size_t row = 0; row < 8; row++) {
        for (size_t col = 0; col < 8; col++) {
            size_t square = (7 - row) * 8 + col;
            if ((bitmap >> square) & 1) {
                printf("1");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

void print_board(Board *board) {
    for (size_t row = 0; row < 8; row++) {
        printf("---------------------------------\n");
        printf("| ");
        for (size_t col = 0; col < 8; col++) {
            size_t square = (7 - row) * 8 + col;
            Piece piece = get_piece(board, square);
            print_piece(piece);
            printf(" | ");
        }
        printf("\n");
    }
    printf("---------------------------------\n");
}

void print_fen(Board *board) {
    char *fen = board_to_fen(board);
    printf("%s\n", fen);
    free(fen);
}
