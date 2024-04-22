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
        Position(std::string fen);



        bool whiteKingsideCastle = true; //remember to make this true again
        bool whiteQueensideCastle = true;
        bool blackKingsideCastle = true;
        bool blackQueensideCastle = true;

        int enPassantTarget = 0;
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
        void loadFromFEN(const std::string& fen);



    private:
        int savedEnPassantTarget = 0;


        void makeMove(Move move);
        void addPiece(Piece piece, int x, int y);
        void capturePiece(int from, int to);
        void unCapturePiece(int from, int to, Piece piece);
        void capturePieceEP(int from, int to);
        void unCapturePieceEP(int from, int to, Piece piece);
        void movePiece(int from, int to);
        void capturePiecePromote(int from, int to, int code);
        void movePiecePromote(int from, int to, int code);

    };

} // Engine

#endif //UNTITLED_POSITION_H
