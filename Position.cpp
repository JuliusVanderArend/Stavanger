//
// Created by Julius on 28/02/2024.
//

#include "Position.h"

namespace Engine {
    Position::Position() {
        pieces = new Bitboard[64];
        pieceNames = new Piece[64];

//        addPiece(Piece::WHITE_ROOK, 0, 0);
//        addPiece(Piece::WHITE_KNIGHT, 1, 0);
//        addPiece(Piece::WHITE_BISHOP, 2, 0);
//        addPiece(Piece::WHITE_QUEEN, 3, 0);
//        addPiece(Piece::WHITE_KING, 4, 0);
//        addPiece(Piece::WHITE_BISHOP, 5, 0);
//        addPiece(Piece::WHITE_KNIGHT, 6, 0);
//        addPiece(Piece::WHITE_ROOK, 7, 0);

//        for (int i = 0; i < 8; ++i) {
//            addPiece(Piece::WHITE_PAWN, i, 1);
//        }
//
//        for (int i = 0; i < 8; ++i) {
//            addPiece(Piece::BLACK_PAWN, i, 6);
//        }

//        addPiece(Piece::BLACK_ROOK, 0, 7);
//        addPiece(Piece::BLACK_KNIGHT, 1, 7);
//        addPiece(Piece::BLACK_BISHOP, 2, 7);
//        addPiece(Piece::BLACK_QUEEN, 3, 7);
//        addPiece(Piece::BLACK_KING, 4, 7);
//        addPiece(Piece::BLACK_BISHOP, 5, 7);
//        addPiece(Piece::BLACK_KNIGHT, 6, 7);
//        addPiece(Piece::BLACK_ROOK, 7, 7);

        //testing
//        addPiece(Piece::WHITE_ROOK, 2, 2);
//        addPiece(Piece::WHITE_BISHOP, 4, 2);
//        addPiece(Piece::WHITE_QUEEN, 6, 2);

        addPiece(Piece::WHITE_PAWN, 3, 3);
        addPiece(Piece::WHITE_PAWN, 3, 4);
        addPiece(Piece::WHITE_PAWN, 3, 5);
        addPiece(Piece::WHITE_PAWN, 4, 3);
        addPiece(Piece::WHITE_PAWN, 4, 4);
        addPiece(Piece::WHITE_PAWN, 4, 5);
        addPiece(Piece::WHITE_PAWN, 5, 3);
        addPiece(Piece::WHITE_PAWN, 5, 4);
        addPiece(Piece::WHITE_PAWN, 5, 5);

    }

    void Position::addPiece(Piece piece, int x, int y) {
        Bitboard newPiece; //mem leak?
        this->occupancy.set(y*8 + x);
        if(piece > 0){
            whiteOccupancy.set(y*8 + x);
        }
        else{
            blackOccupancy.set(y*8 + x);
        }
        newPiece.set(y*8 + x);
        this->pieces[this->numPieces] = newPiece;
        this->pieceNames[this->numPieces] = piece;
        this->numPieces++;
    }

    void Position::movePiece(int index, int x, int y) {
        this->occupancy.set(y*8 + x);
        this->occupancy.reset(this->pieces[index]._Find_first());
        if(this->pieceNames[index] > 0){
            whiteOccupancy.set(y*8 + x);
            this->whiteOccupancy.reset(this->pieces[index]._Find_first());
        }
        else{
            blackOccupancy.set(y*8 + x);
            this->blackOccupancy.reset(this->pieces[index]._Find_first());
        }
        this->pieces[index].reset();
        this->pieces[index].set(y*8 + x);
    }

    int Position::getPieceIndex(Engine::Piece piece, int x, int y) {
        for (int i = 0; i < this->numPieces; ++i) {
            if(this->pieces[i]._Find_first() == x+y*8 && this->pieceNames[i] == piece){
                return i;
            }
        }
        return -1;
    }

    void Position::toArray(int**& boardOut){//converts bitboard representation of a chess board to array representation. (is position:: needed?)

        boardOut = new int*[8]; // an integer representation of a chessboard to be printed 1 = pawn, 2 = knight, 3 = bishop, 4 = rook, 5 = king, 6 = queen ,positive for white pieces, negative for black;
        for (int i = 0; i < 8; ++i) {
            boardOut[i] = new int[8];
            for (int j = 0; j < 8; ++j) {
                // Initialize or assign values to the array
                boardOut[i][j] = 0;
            }
        }

        for (int i = 0; i < this->numPieces; ++i) {
            Bitboard piece = this->pieces[i];
            int x = piece._Find_first() % 8;
            int y = piece._Find_first() / 8;
            boardOut[x][y] = this->pieceNames[i];
        }
    }

    void Position::drawBitboard(Bitboard bitboard) {
        for (int i = 7; i >= 0; --i) {
            for (int j = 0; j < 8; ++j) {
                std::cout << " " + std::to_string(bitboard.test(j + i * 8)) + " ";//?
            }
            std::cout << std::endl;
        }
    }

    void Position::debugDraw(std::string mode){
        if(mode == "occupancy") {
            drawBitboard(this->occupancy);
        }
        if(mode == "white") {
            drawBitboard(this->whiteOccupancy);
        }
        if(mode == "black") {
            drawBitboard(this->blackOccupancy);
        }
    }

    void Position::draw(){
        int** board;
        this->toArray(board);

        for (int i = 7; i >=0; --i) {
            for (int j = 0; j < 8; ++j) {
                switch (board[j][i]) {
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
        for (int i = 0; i < 8; ++i) {
            delete[] board[i];
        }
        delete[] board;
    }


} // Engine