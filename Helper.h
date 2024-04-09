//
// Created by Julius on 02/03/2024.
//



#ifndef UNTITLED_HELPER_H
#define UNTITLED_HELPER_H

#include <iostream>
#include <cstdint>
#include <bitset>

typedef uint64_t Bitboard;
//typedef std::bitset<32> Move;
typedef  uint32_t Move; //faster than uint16??? WHY??!

namespace Engine {
    enum Piece {
        WHITE_KING = 6,
        WHITE_QUEEN = 5,
        WHITE_PAWN = 1,
        WHITE_BISHOP = 3,
        WHITE_KNIGHT = 2,
        WHITE_ROOK = 4,
        BLACK_KING = -6,
        BLACK_QUEEN = -5,
        BLACK_PAWN = -1,
        BLACK_BISHOP = -3,
        BLACK_KNIGHT = -2,
        BLACK_ROOK = -4,
        NONE = 0
    };
    bool isSliding(Piece piece);
    template<std::size_t N>
    std::string toCoordinates(int index);
    void printMove(uint32_t move);
    inline uint64_t set(uint64_t& bitboard, uint8_t index);
    inline uint64_t clear(uint64_t& bitboard, uint8_t index);
    inline uint64_t move(uint64_t& bitboard, uint8_t from,uint8_t to);

} // Helper

#endif //UNTITLED_HELPER_H
