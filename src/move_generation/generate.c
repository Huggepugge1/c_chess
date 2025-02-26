#include "generate.h"
#include "../vector.h"
#include "bishop.h"
#include "king.h"
#include "knight.h"
#include "pawn.h"
#include "queen.h"
#include "rook.h"

MoveVector *generate_moves(Board *board) {
    MoveVector *moves = new_move_vector();

    MoveVector *pawn_moves = generate_pawn_moves(board);
    append_move_vector(moves, pawn_moves);
    free_move_vector(pawn_moves);

    MoveVector *rook_moves = generate_rook_moves(board);
    append_move_vector(moves, rook_moves);
    free_move_vector(rook_moves);

    MoveVector *knight_moves = generate_knight_moves(board);
    append_move_vector(moves, knight_moves);
    free_move_vector(knight_moves);

    MoveVector *bishop_moves = generate_bishop_moves(board);
    append_move_vector(moves, bishop_moves);
    free_move_vector(bishop_moves);

    MoveVector *queen_moves = generate_queen_moves(board);
    append_move_vector(moves, queen_moves);
    free_move_vector(queen_moves);

    MoveVector *king_moves = generate_king_moves(board);
    append_move_vector(moves, king_moves);
    free_move_vector(king_moves);

    return moves;
}
