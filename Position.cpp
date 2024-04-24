//
// Created by Julius on 28/02/2024.
//

#include <sstream>
#include "Position.h"

namespace Engine {
    Position::Position(std::string fen) {
        loadFromFEN(fen);
    }

    // Method to load position from FEN string
    void Position::loadFromFEN(const std::string& fen) {
        std::fill(pieceNames.begin(), pieceNames.end(), Piece::NONE);
        std::fill(pieceOccupancy.begin(), pieceOccupancy.end(), 0);
        occupancy = 0;
        whiteOccupancy = 0;
        blackOccupancy = 0;



        size_t spacePos = fen.find(' ');
        if (spacePos == std::string::npos) {
            std::cerr << "Invalid FEN format: no space found" << std::endl;
            return;
        }

        std::string piecePlacement = fen.substr(0, spacePos);

        std::string auxInfo = fen.substr(spacePos + 1);

        int file = 0;
        int rank = 7;

        for (char c : piecePlacement) {
            if (c == '/') {
                rank--;
                file = 0;
                continue;
            }
            if (isdigit(c)) {
                file += c - '0';
            } else {
                switch (c) {
                    case 'p': addPiece(Piece::BLACK_PAWN, file, rank); break;
                    case 'r': addPiece(Piece::BLACK_ROOK, file, rank); break;
                    case 'n': addPiece(Piece::BLACK_KNIGHT, file, rank); break;
                    case 'b': addPiece(Piece::BLACK_BISHOP, file, rank); break;
                    case 'q': addPiece(Piece::BLACK_QUEEN, file, rank); break;
                    case 'k': addPiece(Piece::BLACK_KING, file, rank); break;
                    case 'P': addPiece(Piece::WHITE_PAWN, file, rank); break;
                    case 'R': addPiece(Piece::WHITE_ROOK, file, rank); break;
                    case 'N': addPiece(Piece::WHITE_KNIGHT, file, rank); break;
                    case 'B': addPiece(Piece::WHITE_BISHOP, file, rank); break;
                    case 'Q': addPiece(Piece::WHITE_QUEEN, file, rank); break;
                    case 'K': addPiece(Piece::WHITE_KING, file, rank); break;
                    default: break;
                }
                file++;
            }
        }

        whiteKingsideCastle = false;
        whiteQueensideCastle = false;
        blackKingsideCastle = false;
        blackQueensideCastle = false;

        for (auto it = auxInfo.begin(); it != auxInfo.end(); ++it) {
            char c = *it;
            switch (c) {
                case 'w': whiteToMove = true; break;
                case 'b': whiteToMove = false; break;
                case 'K': whiteKingsideCastle = true; break;
                case 'Q': whiteQueensideCastle = true; break;
                case 'k': blackKingsideCastle = true; break;
                case 'q': blackQueensideCastle = true; break;

                default: break;
            }
            if(isalpha(c)&& isdigit(*(std::next(it)))){ //checking for a ep target square, kind of a hack, could be bad.
                enPassantTarget = toIndex(std::string(it, it+2));
            }
        }

        friendlyOccupancy = whiteToMove?&whiteOccupancy:&blackOccupancy;
        enemyOccupancy = whiteToMove?&blackOccupancy:&whiteOccupancy;
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
        enPassantTarget = 0; //how do deal with resetting eptarget when unmaking moves recursively?

        Bitboard* temp = friendlyOccupancy; //swap friendly/enemy occupancy ptrs.
        friendlyOccupancy = enemyOccupancy;
        enemyOccupancy = temp;

    }

//    void Position::unmakeMove(Move move){
//
//    }


    void Position::addPiece(Piece piece, int x, int y) {
        set(occupancy,y*8 + x);
        if(piece > 6){
            set(whiteOccupancy,y*8 + x);
        }
        else{
            set(blackOccupancy,y*8 + x);
        }
        pieceNames[x+y*8] = piece;
        set(pieceOccupancy[piece],x+y*8);
//        drawBitboard(pieceOccupancy[piece]);
//        std::cout << piece << std::endl;
    }

    void Position::capturePiece(int from, int to) {
        clear(occupancy,from); //clear occupancy of from square
        move(*friendlyOccupancy,from,to); //move friendly occupancy to destination
        clear(*enemyOccupancy,to); //clear enemy occupancy in destination

        clear(pieceOccupancy[pieceNames[to]],to); //clear captured piece bit

        move(pieceOccupancy[pieceNames[from]],from,to); //move capturing piece bit

        pieceNames[to] = pieceNames[from];//update piece names
        pieceNames[from] = Piece::NONE;
    }

    void Position::unCapturePiece(int from, int to, Piece piece){
        set(occupancy, from);
        move(*friendlyOccupancy,to,from);
        set(*enemyOccupancy,to);

        set(pieceOccupancy[piece],to); //clear captured piece bit

        move(pieceOccupancy[pieceNames[to]],to,from); //move capturing piece bit

        pieceNames[from] = pieceNames[to];
        pieceNames[to] = piece;
    }

    void Position::capturePieceEP(int from, int to) { //be very careful to get the logic right here, potential to optimise this by storing capture square instead of en passant target
        int captureSquare = enPassantTarget+ (whiteToMove?-8:8);
        move(occupancy,from,to); //move occupancy to destination
        move(*friendlyOccupancy,from,to); //move friendly occupancy to destination
        clear(*enemyOccupancy,captureSquare); //clear enemy occupancy in en passant square
        clear(occupancy,captureSquare);

        clear(pieceOccupancy[pieceNames[captureSquare]],captureSquare); //clear captured piece bit

        move(pieceOccupancy[pieceNames[from]],from,to); //move capturing piece bit

        pieceNames[to] = pieceNames[from];//update piece names
        pieceNames[from] = Piece::NONE;
        pieceNames[captureSquare] = Piece::NONE;
    }

    void Position::unCapturePieceEP(int from, int to, Engine::Piece piece) {
        move(occupancy,from,to); //move occupancy to destination
        move(*friendlyOccupancy,from,to); //move friendly occupancy to destination
        clear(*enemyOccupancy,enPassantTarget); //clear enemy occupancy in en passant square

        clear(pieceOccupancy[pieceNames[enPassantTarget]],enPassantTarget); //clear captured piece bit

        move(pieceOccupancy[pieceNames[from]],from,to); //move capturing piece bit

        pieceNames[to] = pieceNames[from];//update piece names
        pieceNames[from] = Piece::NONE;
        pieceNames[enPassantTarget] = Piece::NONE;
    }

    void Position::movePiece(int from, int to) {
        move(occupancy,from,to);
        move(*friendlyOccupancy,from,to);
        move(pieceOccupancy[pieceNames[from]],from,to);
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

        clear(pieceOccupancy[pieceNames[from]],from); //create promoted piece
        set(pieceOccupancy[promoted],to);

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

        clear(pieceOccupancy[pieceNames[from]],from); //create promoted piece
        set(pieceOccupancy[promoted],to);

        clear(*enemyOccupancy,to);
        clear(pieceOccupancy[pieceNames[to]],to); //clear captured piece bit

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
                        std::cout << " p ";  // Pawn
                        break;
                    case 2:
                        std::cout << " n ";  // Knight
                        break;
                    case 3:
                        std::cout << " b ";  // Bishop
                        break;
                    case 4:
                        std::cout << " r ";  // Rook
                        break;
                    case 5:
                        std::cout << " q ";  // Queen
                        break;
                    case 6:
                        std::cout << " k ";  // King
                        break;
                    case 7:
                        std::cout << " P ";  // Pawn (black)
                        break;
                    case 8:
                        std::cout << " N ";  // Knight (black)
                        break;
                    case 9:
                        std::cout << " B ";  // Bishop (black)
                        break;
                    case 10:
                        std::cout << " R ";  // Rook (black)
                        break;
                    case 11:
                        std::cout << " Q ";  // Queen (black)
                        break;
                    case 12:
                        std::cout << " K ";  // King (black)
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