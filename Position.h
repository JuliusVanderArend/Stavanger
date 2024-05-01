//
// Created by Julius on 28/02/2024.
//


#ifndef UNTITLED_POSITION_H
#define UNTITLED_POSITION_H

#include <cstdint>
#include <array>
#include <vector>
#include <stack>
#include "Helper.h"


namespace Engine {

    struct irreversibleAspect{
        bool whiteKingsideCastle;
        bool whiteQueensideCastle;
        bool blackKingsideCastle;
        bool blackQueensideCastle;

        int enPassantTarget = 0;
    };

    class Position {
    public:
        Position(std::string fen);

        bool whiteKingsideCastle = true;
        bool whiteQueensideCastle = true;
        bool blackKingsideCastle = true;
        bool blackQueensideCastle = true;

        int enPassantTarget = 0;
        bool whiteToMove = true;

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
        void makeMove(Move move);
        void unMakeMove(Move move);

        bool equals(const Position& position);

        Position(const Position& other){
            whiteKingsideCastle = other.whiteKingsideCastle;
            whiteQueensideCastle = other.whiteQueensideCastle;
            blackKingsideCastle = other.blackKingsideCastle;
            blackQueensideCastle = other.blackQueensideCastle;
            enPassantTarget = other.enPassantTarget;
            whiteToMove = other.whiteToMove;
            pieceOccupancy = other.pieceOccupancy;
            pieceNames = other.pieceNames;
            occupancy = other.occupancy;
            whiteOccupancy = other.whiteOccupancy;
            blackOccupancy = other.blackOccupancy;
            pieceOccupancy = other.pieceOccupancy;
            pieceNames = other.pieceNames;
            friendlyOccupancy = whiteToMove ? &whiteOccupancy : &blackOccupancy;
            enemyOccupancy = whiteToMove ? &blackOccupancy : &whiteOccupancy;
        }

    private:
        std::stack<irreversibleAspect> irreversibleAspectStack;

        Piece promotionCodeToPiece(int code);

        void addPiece(Piece piece, int x, int y);
        void capturePiece(int from, int to);
        void unCapturePiece(int from, int to, Piece piece);
        void capturePieceEP(int from, int to);
        void unCapturePieceEP(int from, int to, Piece piece);
        void movePiece(int from, int to);
        void unMovePiece(int from, int to);
        void capturePiecePromote(int from, int to, int code);
        void unCapturePiecePromote(int from, int to,int code, Engine::Piece captured);
        void movePiecePromote(int from, int to, int code);
        void unMovePiecePromote(int from, int to, int code);

    };

} // Engine

#endif //UNTITLED_POSITION_H
