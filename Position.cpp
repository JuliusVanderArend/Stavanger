//
// Created by Julius on 28/02/2024.
//

#include "Position.h"

namespace Engine {
    Position::Position() {
        std::fill(pieceNames.begin(), pieceNames.end(), Piece::NONE);
        std::fill(pieceOccupancy.begin(), pieceOccupancy.end(), 0);
        // starting position
//        addPiece(Piece::WHITE_ROOK, 0, 0);
//        addPiece(Piece::WHITE_KNIGHT, 1, 0);
//        addPiece(Piece::WHITE_BISHOP, 2, 0);
//        addPiece(Piece::WHITE_QUEEN, 3, 0);
//        addPiece(Piece::WHITE_KING, 4, 0);
//        addPiece(Piece::WHITE_BISHOP, 5, 0);
//        addPiece(Piece::WHITE_KNIGHT, 6, 0);
//        addPiece(Piece::WHITE_ROOK, 7, 0);
//
//        for (int i = 0; i < 8; ++i) {
//            addPiece(Piece::WHITE_PAWN, i, 1);
//        }
//
        for (int i = 0; i < 8; ++i) {
            addPiece(Piece::BLACK_PAWN, i, 6);
        }
//
//        addPiece(Piece::BLACK_ROOK, 0, 7);
//        addPiece(Piece::BLACK_KNIGHT, 1, 7);
//        addPiece(Piece::BLACK_BISHOP, 2, 7);
//        addPiece(Piece::BLACK_QUEEN, 3, 7);
//        addPiece(Piece::BLACK_KING, 4, 7);
//        addPiece(Piece::BLACK_BISHOP, 5, 7);
//        addPiece(Piece::BLACK_KNIGHT, 6, 7);
//        addPiece(Piece::BLACK_ROOK, 7, 7);

        //testing
        addPiece(Piece::WHITE_ROOK, 2, 2);
        addPiece(Piece::WHITE_BISHOP, 4, 2);
//        addPiece(Piece::WHITE_QUEEN, 6, 2);
        addPiece(Piece::WHITE_KING, 7, 1);
        addPiece(Piece::WHITE_KNIGHT, 1, 4);
        addPiece(Piece::WHITE_PAWN, 1, 1);

//        addPiece(Piece::WHITE_PAWN, 3, 3);
//        addPiece(Piece::WHITE_PAWN, 3, 4);
//        addPiece(Piece::WHITE_PAWN, 3, 5);
//        addPiece(Piece::WHITE_PAWN, 4, 3);
//        addPiece(Piece::WHITE_PAWN, 4, 4);
//        addPiece(Piece::WHITE_PAWN, 4, 5);
//        addPiece(Piece::WHITE_PAWN, 5, 3);
//        addPiece(Piece::WHITE_PAWN, 5, 4);
//        addPiece(Piece::WHITE_PAWN, 5, 5);

    }

    bool Position::makeMove(Move move){
        uint8_t from = (move & 0xFC0) >> 6;
        uint8_t to = move & 0x3F;
        uint8_t code = (move & 0xF000) >> 12;

        enPassantTarget = 0; //how do deal with reseting eptarget when unmaking moves?
        movePiece(from,to); //does all the bitboard manipulation need to happen as well? could be put off for better perf?

//        switch (code) {
//            case 1: //is double pawn push so set en passant target square
//                enPassantTarget = to + (whiteToMove? -8:8);
//                break;
//            case 2: //Kingside castle
//
//                break;
//
//            case 3: //Queenside castle
//
//        }
        return true;
    }

    bool Position::squareIsAttacked(int square){ //maybe make return index of attacking piece?
//        if(whiteToMove){
//
//        }else{
//
//        }
        return false;
    }

    void Position::addPiece(Piece piece, int x, int y) {
        set(occupancy,y*8 + x);
        if(piece > 0){
            set(whiteOccupancy,y*8 + x);
        }
        else{
            set(blackOccupancy,y*8 + x);
        }
        pieceNames[x+y*8] = piece;
        set(pieceOccupancy[piece+6],x+y*8);
        drawBitboard(pieceOccupancy[piece+6]);
        std::cout << piece << std::endl;
    }

    void Position::capturePiece(Piece piece, int square) {
        clear(occupancy,square);//maybe consider getting rid of total occupancy if its not used much
        clear(*enemyOccupancy,square);
        clear(pieceOccupancy[piece+6],square);
        pieceNames[square] = Piece::NONE;//maybe I don't have to do this?
    }

    void Position::movePiece(int from, int to) {
        move(occupancy,from,to);
        move(*friendlyOccupancy,from,to);
        move(pieceOccupancy[pieceNames[from]+6],from,to);
        pieceNames[to] = pieceNames[from];//maybe I don't have to do this?
        pieceNames[from] = Piece::NONE;
    }

//    int Position::getPieceIndex(Engine::Piece piece, int x, int y) {
//        for (int i = 0; i < numPieces; ++i) {
//            if(pieces[i]._Find_first() == x+y*8 && pieceNames[i] == piece){
//                return i;
//            }
//        }
//        return -1;
//    }




    void Position::debugDraw(std::string mode){
        if(mode == "occupancy") {
            drawBitboard(occupancy);
        }
        if(mode == "white") {
            drawBitboard(whiteOccupancy);
        }
        if(mode == "black") {
            drawBitboard(blackOccupancy);
        }
    }

    void Position::draw(){
        for (int i = 7; i >=0; --i) {
            for (int j = 0; j < 8; ++j) {
                switch (pieceNames[i*8+j]) {
                    case 0:
                        std::cout << " . ";  // Empty square
                        break;
                    case 1:
                        std::cout << " P ";  // Pawn
                        break;
                    case 2:
                        std::cout << " N ";  // Knight
                        break;
                    case 3:
                        std::cout << " B ";  // Bishop
                        break;
                    case 4:
                        std::cout << " R ";  // Rook
                        break;
                    case 5:
                        std::cout << " Q ";  // Queen
                        break;
                    case 6:
                        std::cout << " K ";  // King
                        break;
                    case -1:
                        std::cout << " p ";  // Pawn (black)
                        break;
                    case -2:
                        std::cout << " n ";  // Knight (black)
                        break;
                    case -3:
                        std::cout << " b ";  // Bishop (black)
                        break;
                    case -4:
                        std::cout << " r ";  // Rook (black)
                        break;
                    case -5:
                        std::cout << " q ";  // Queen (black)
                        break;
                    case -6:
                        std::cout << " k ";  // King (black)
                        break;
                    default:
                        std::cerr << "Unknown piece on the board!";
                        break;
                }
            }
            std::cout << std::endl;
        }
    }
}