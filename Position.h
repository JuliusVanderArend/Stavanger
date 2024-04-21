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

        bool whiteKingsideCastle = false; //remember to make this true again
        bool whiteQueensideCastle = false;
        bool blackKingsideCastle = false;
        bool blackQueensideCastle = false;

        int enPassantTarget = 0;
        int savedEnPassantTarget = 0;
        bool whiteToMove = true;
        int numPieces = 0; //get rid of this at some point

        std::array<Bitboard,13> pieceOccupancy;

        std::array<Piece,64> pieceNames;
        Bitboard occupancy = 0;
        Bitboard whiteOccupancy = 0; //color occupancy not including kings???
        Bitboard blackOccupancy = 0;
        Bitboard* friendlyOccupancy = &whiteOccupancy; // does this work? remember to change when make move
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
