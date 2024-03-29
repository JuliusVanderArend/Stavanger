//
// Created by Julius on 28/02/2024.
//


#ifndef UNTITLED_POSITION_H
#define UNTITLED_POSITION_H

#include <cstdint>
#include <bitset>
#include "Helper.h"

typedef std::bitset<64> Bitboard;


namespace Engine {

    class Position {
    public:
        Position();

        bool whiteCanCastle = true;
        bool blackCanCastle = true;
        bool whiteToMove = true;
        int numPieces = 0;

        Bitboard* pieces;
        Bitboard occupancy;
        Bitboard whiteOccupancy;
        Bitboard blackOccupancy;
        Piece* pieceNames;

        void draw();
        void debugDraw(std::string mode);
        void drawBitboard(Bitboard bitboard);
        void toArray(int**& boardOut);

        void addPiece(Piece piece, int x, int y);
        void movePiece(int index, int x, int y);
        int getPieceIndex(Piece piece, int x, int y); //try to avoid or optimise this as currently looping over all pieces

    private:

    };

} // Engine

#endif //UNTITLED_POSITION_H
