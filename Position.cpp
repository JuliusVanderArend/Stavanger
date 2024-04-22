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

    void Position::makeMove(Move move){
        uint8_t from = (move & 0xFC0) >> 6;
        uint8_t to = move & 0x3F;
        uint8_t code = (move & 0xF000) >> 12;

        int toMove = whiteToMove?1:-1;


        switch (code) { // refactor this later, probably a more elegant and less branchy way of doing it.
            case 0://quiet moves
                movePiece(from,to); //does all the bitboard manipulation need to happen as well? could be put off for better perf?
                break;
            case 1: //is double pawn push so set en passant target square
                movePiece(from,to);
                enPassantTarget = to + (whiteToMove? -8:8);
                break;
            case 2: //Kingside 
                movePiece(from,to);
                movePiece(whiteToMove?7:63,whiteToMove?5:61); //move rook and unset castling rights
                if(whiteToMove){
                    whiteQueensideCastle = false;
                    whiteKingsideCastle = false;
                }
                else{
                    blackQueensideCastle = false;
                    blackKingsideCastle = false;
                }
                break;
            case 3: //Queenside castle
                movePiece(from,to);
                movePiece(whiteToMove?0:56,whiteToMove?3:59); //move rook and unset castling rights
                if(whiteToMove){
                    whiteQueensideCastle = false;
                    whiteKingsideCastle = false;
                }
                else{
                    blackQueensideCastle = false;
                    blackKingsideCastle = false;
                }
                break;
            case 4: //capture
                capturePiece(from,to);
                break;
            case 5: //enpessant capture
                capturePieceEP(from,to);
                break;
            case 8: // simple promotions
            case 9:
            case 10:
            case 11:
                movePiecePromote(from,to,code);
                break;
            case 12: // capture promotions
            case 13:
            case 14:
            case 15:
                capturePiecePromote(from,to,code);
                break;
            default:
                break;
        }

        savedEnPassantTarget = enPassantTarget;
        enPassantTarget = 0; //how do deal with resetting eptarget when unmaking moves?
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

    void Position::capturePiece(int from, int to) {
        clear(occupancy,from); //clear occupancy of from square
        move(*friendlyOccupancy,from,to); //move friendly occupancy to destination
        clear(*enemyOccupancy,to); //clear enemy occupancy in destination

        clear(pieceOccupancy[pieceNames[to]+6],to); //clear captured piece bit

        move(pieceOccupancy[pieceNames[from]+6],from,to); //move capturing piece bit

        pieceNames[to] = pieceNames[from];//update piece names
        pieceNames[from] = Piece::NONE;
    }

    void Position::capturePieceEP(int from, int to) { //be very carefull to get the logic right here
        move(occupancy,from,to); //move occupancy to destination
        move(*friendlyOccupancy,from,to); //move friendly occupancy to destination
        clear(*enemyOccupancy,enPassantTarget); //clear enemy occupancy in en passant square

        clear(pieceOccupancy[pieceNames[enPassantTarget]+6],enPassantTarget); //clear captured piece bit

        move(pieceOccupancy[pieceNames[from]+6],from,to); //move capturing piece bit

        pieceNames[to] = pieceNames[from];//update piece names
        pieceNames[from] = Piece::NONE;
        pieceNames[enPassantTarget] = Piece::NONE;
    }

    void Position::movePiece(int from, int to) {
        move(occupancy,from,to);
        move(*friendlyOccupancy,from,to);
        move(pieceOccupancy[pieceNames[from]+6],from,to);
        pieceNames[to] = pieceNames[from];//maybe I don't have to do this?
        pieceNames[from] = Piece::NONE;
    }

    void Position::movePiecePromote(int from, int to, int code) {
        Piece promoted = Piece::NONE;
        switch (code) {
            case 8:
                promoted = whiteToMove? Piece::WHITE_KNIGHT:Piece::BLACK_KNIGHT;
                break;
            case 9:
                promoted = whiteToMove? Piece::WHITE_BISHOP:Piece::BLACK_BISHOP;
                break;
            case 10:
                promoted = whiteToMove? Piece::WHITE_ROOK:Piece::BLACK_ROOK;
                break;
            case 11:
                promoted = whiteToMove? Piece::WHITE_QUEEN:Piece::BLACK_QUEEN;
                break;
            default:
                std::cout << "ERROR wrong promotion code" << std::endl;
        }


        move(occupancy,from,to);
        move(*friendlyOccupancy,from,to);

        clear(pieceOccupancy[pieceNames[from]+6],from); //create promoted piece
        set(pieceOccupancy[promoted+6],to);

        pieceNames[to] = promoted;//maybe I don't have to do this?
        pieceNames[from] = Piece::NONE;
    }

    void Position::capturePiecePromote(int from, int to, int code) {
        Piece promoted = Piece::NONE;
        switch (code) {
            case 8:
                promoted = whiteToMove? Piece::WHITE_KNIGHT:Piece::BLACK_KNIGHT;
                break;
            case 9:
                promoted = whiteToMove? Piece::WHITE_BISHOP:Piece::BLACK_BISHOP;
                break;
            case 10:
                promoted = whiteToMove? Piece::WHITE_ROOK:Piece::BLACK_ROOK;
                break;
            case 11:
                promoted = whiteToMove? Piece::WHITE_QUEEN:Piece::BLACK_QUEEN;
                break;
            default:
                std::cout << "ERROR wrong promotion code" << std::endl;
        }


        move(occupancy,from,to);
        move(*friendlyOccupancy,from,to);

        clear(pieceOccupancy[pieceNames[from]+6],from); //create promoted piece
        set(pieceOccupancy[promoted+6],to);

        clear(*enemyOccupancy,to);
        clear(pieceOccupancy[pieceNames[to]+6],to); //clear captured piece bit

        pieceNames[to] = promoted;//maybe I don't have to do this?
        pieceNames[from] = Piece::NONE;
    }



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