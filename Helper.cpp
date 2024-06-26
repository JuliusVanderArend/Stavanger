//
// Created by Julius on 02/03/2024.
//

#include <map>
#include "Helper.h"

namespace Engine {

    std::string toCoordinates(int index) {
        int row = (index / 8); // Calculate row (rank) from index
        int col = index % 8;       // Calculate column (file) from index

        // Convert row and column to chess coordinates (e.g      ., a1, b2, ..., h8)
        char file = 'a' + col;
        char rank = '1' + row;

        // Return the chess coordinate string
        return std::string(1, file) + std::string(1, rank);
    }

    int toIndex(const std::string& square) {
        int file = square[0] - 'a'; // Convert the file (letter) to a number (0-7)
        int rank = square[1] - '1'; // Convert the rank (number) to a number (0-7)
        return rank * 8 + file; // Calculate the board index
    }

    std::tuple<int, int, int> toMove(const std::string& moveString){
//        if(moveString == "O-O"){
//            return 6 | (4 << 6) |
//        }
//        if(moveString == "o-o"){
//
//        }
//        if(moveString == "O-O-O"){
//
//        }
//        if(moveString == "o-o-o"){
//
//        }


        int from =  ((moveString[0] - 'a') + ((moveString[1] - '1') * 8));
        int to = (moveString[2] - 'a') + ((moveString[3] - '1') * 8);
        int promotion = 0;

        if (moveString.length() == 5) {
            switch (moveString[4]) {
                case 'n':
                    promotion = 8; // Knight
                    break;
                case 'b':
                    promotion = 9; // Bishop
                    break;
                case 'r':
                    promotion = 10; // Rook
                    break;
                case 'q':
                    promotion = 11; // Queen
                    break;
                default:
                    break;
            }
        }
        return  std::make_tuple(from, to, promotion);
    }

    void drawBitboard(Bitboard bitboard) {
        std::cout << "=======================" <<std::endl;
        for (int i = 7; i >= 0; --i) {
            for (int j = 0; j < 8; ++j) {
                std::cout << " " + std::to_string(test(bitboard,i*8+j)) + " ";//?
            }
            std::cout << std::endl;
        }
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