//
// Created by Julius on 28/02/2024.
//


#ifndef UNTITLED_POSITION_H
#define UNTITLED_POSITION_H

#include <cstdint>
#include <array>
#include "Helper.h"



namespace Engine {

    class Position {
    public:
        Position();

        bool whiteKingsideCastle = true;
        bool whiteQueensideCastle = true;
        bool blackKingsideCastle = true;
        bool blackQueensideCastle = true;

        int enPassantTarget = 0;
        bool whiteToMove = true;
        int numPieces = 0;

        std::array<Bitboard,64> pieces;
        Bitboard occupancy;
        Bitboard whiteOccupancy;
        Bitboard blackOccupancy;
        std::array<Piece,64> pieceNames;

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
