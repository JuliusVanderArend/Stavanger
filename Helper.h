//
// Created by Julius on 02/03/2024.
//



#ifndef UNTITLED_HELPER_H
#define UNTITLED_HELPER_H

#include <iostream>


namespace Engine {
    enum Piece {
        WHITE_KING = 6,
        WHITE_QUEEN = 5,
        WHITE_PAWN = 1,
        WHITE_BISHOP = 3,
        WHITE_KNIGHT = 2,
        WHITE_ROOK = 4,
        BLACK_KING = -6,
        BLACK_QUEEN = -5,
        BLACK_PAWN = -1,
        BLACK_BISHOP = -3,
        BLACK_KNIGHT = -2,
        BLACK_ROOK = -4
    };
    bool isSliding(Piece piece);
} // Helper

#endif //UNTITLED_HELPER_H
