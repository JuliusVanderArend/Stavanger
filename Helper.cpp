//
// Created by Julius on 02/03/2024.
//

#include <map>
#include "Helper.h"

namespace Engine {
    bool isSliding(Engine::Piece piece){
        return 3 <= abs(piece) &&  abs(piece)<= 5;
    }

    inline void set(uint64_t& bitboard, uint8_t index) {
        bitboard |= (1ULL << index);
    }

    inline void clear(uint64_t& bitboard, uint8_t index) {
        bitboard &= ~(1ULL << index);
    }

    inline void move(uint64_t& bitboard, uint8_t from, uint8_t to) {
        bitboard ^= (1ULL << from | 1ULL << to);
    }


    std::string toCoordinates(int index) {
        int row = (index / 8); // Calculate row (rank) from index
        int col = index % 8;       // Calculate column (file) from index

        // Convert row and column to chess coordinates (e.g., a1, b2, ..., h8)
        char file = 'a' + col;
        char rank = '1' + row;

        // Return the chess coordinate string
        return std::string(1, file) + std::string(1, rank);
    }

    void printMove(Move move){
//        static std::map<Piece, std::string> pieceNames = {
//                {Piece::WHITE_KING, "White King"},
//                {Piece::WHITE_QUEEN, "White Queen"},
//                {Piece::WHITE_ROOK, "White Rook"},
//                {Piece::WHITE_BISHOP, "White Bishop"},
//                {Piece::WHITE_KNIGHT, "White Knight"},
//                {Piece::WHITE_PAWN, "White Pawn"},
//                {Piece::BLACK_KING, "Black King"},
//                {Piece::BLACK_QUEEN, "Black Queen"},
//                {Piece::BLACK_ROOK, "Black Rook"},
//                {Piece::BLACK_BISHOP, "Black Bishop"},
//                {Piece::BLACK_KNIGHT, "Black Knight"},
//                {Piece::BLACK_PAWN, "Black Pawn"}
//        };
        if(move ==0){
            std::cout <<"null move"<< std::endl;
        }
        else if(move & (1 <<14)) {
            std::cout << "capture from " + toCoordinates((move >> 6) & 0x3f) + " to " + toCoordinates(move & 0x3f)<< std::endl;
        }
        else{
            std::cout << "move from " + toCoordinates((move >> 6) & 0x3f) + " to " + toCoordinates(move & 0x3f)<< std::endl;
        }
//        for (int i = 32 - 1; i >= 0; --i) {
//            // Check if the i-th bit is set
//            if (move & (1 << i)) {
//                std::cout << "1";
//            } else {
//                std::cout << "0";
//            }
//
//            // Add spacing for better readability
//        }
        std::cout << std::endl;
    }

} // Helper