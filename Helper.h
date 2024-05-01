//
// Created by Julius on 02/03/2024.
//



#ifndef UNTITLED_HELPER_H
#define UNTITLED_HELPER_H

#include <iostream>
#include <cstdint>



typedef uint64_t Bitboard;
//typedef std::bitset<32> Move;
typedef  uint32_t Move; //faster than uint16??? WHY??!

namespace Engine {
    enum Piece {
        WHITE_KING = 12,
        WHITE_QUEEN = 11,
        WHITE_PAWN = 7,
        WHITE_BISHOP = 9,
        WHITE_KNIGHT = 8,
        WHITE_ROOK = 10,
        BLACK_KING = 6,
        BLACK_QUEEN = 5,
        BLACK_PAWN = 1,
        BLACK_BISHOP = 3,
        BLACK_KNIGHT = 2,
        BLACK_ROOK = 4,
        NONE = 0
    };


    template<std::size_t N>
    std::string toCoordinates(int index);
    int toIndex(const std::string& square);
    std::tuple<int, int, int> toMove(const std::string& move);
    void printMove(uint32_t move);
    void drawBitboard(Bitboard bitboard);


    inline void set(Bitboard& bitboard, uint8_t index) {
        bitboard |= (1ULL << index);
    }

    inline void clear(Bitboard& bitboard, uint8_t index) {
        bitboard &= ~(1ULL << index);
    }

    inline void move(Bitboard& bitboard, uint8_t from, uint8_t to) {
        bitboard ^= (1ULL << from | 1ULL << to);
    }

    inline bool test(Bitboard& bitboard, uint8_t square) {
        return bitboard & (1ULL << square);
    }

} // Helper

#endif //UNTITLED_HELPER_H
