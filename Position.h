//
// Created by Julius on 28/02/2024.
//


#ifndef UNTITLED_POSITION_H
#define UNTITLED_POSITION_H

#include <cstdint>
#include <array>
#include <vector>
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
        int numPieces = 0; //get rid of this at some point

        std::array<Bitboard,13> pieceOccupancy;

        std::array<Piece,64> pieceNames;
        Bitboard occupancy;
        Bitboard whiteOccupancy;
        Bitboard blackOccupancy;
        Bitboard* friendlyOccupancy = &whiteOccupancy; // does this work?
        Bitboard* enemyOccupancy = &blackOccupancy;




        void draw();
        void debugDraw(std::string mode);
        void toArray(int**& boardOut);

        void addPiece(Piece piece, int x, int y);
        void capturePiece(Piece piece, int square);
        void movePiece(int from, int to);

        int getPieceIndex(Piece piece, int x, int y); //try to avoid or optimise this as currently looping over all pieces
        bool makeMove(Move move);
        bool squareIsAttacked(int square);

    private:

    };

} // Engine

#endif //UNTITLED_POSITION_H
