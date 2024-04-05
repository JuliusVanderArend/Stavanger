//
// Created by Julius on 03/03/2024.
//


#ifndef UNTITLED_MOVEGENERATOR_H
#define UNTITLED_MOVEGENERATOR_H

#include "Position.h"
#include "Helper.h"

namespace Engine {
    struct magicEntry{
        int indexSize;
        uint64_t magic;
    };

    class MoveGenerator {
    public:
        MoveGenerator();
        Bitboard* generateMoves(Position* position);

    private:
        Bitboard generateMagicMaskRay(int direction, int squareIndex);
        Bitboard generateRookMagicMask(int squareIndex);
        Bitboard generateBishopMagicMask(int squareIndex);
        Bitboard floodFill(int direction,  int squareIndex, Bitboard blockers);
        Bitboard floodfillRookAttacks(int position,Bitboard blockers);
        Bitboard floodfillBishopAttacks(int position,Bitboard blockers);
//        void populateMagicLUT(bool isBishop, int squareIndex, int shift,uint64_t magic, Bitboard*& table);
        void initalizeMagics();
        int validateMagic(bool isBishop, int squareIndex, uint64_t magic, Bitboard*& table, uint32_t**& movesetTable);

        void attackMapToMoveset(Bitboard attackMap, uint32_t squareIndex, uint32_t*& moves);

        // sliding piece move generation lookup tables for each direction, indexed as [direction(N->W)][square][blocker arrangement]


        magicEntry rookMagics[64];
        magicEntry bishopMagics[64];

        Bitboard* rookMagicLUT[64];
        Bitboard* bishopMagicLUT[64];
        uint32_t** rookMagicMovesets[64];
        uint32_t** bishopMagicMovesets[64];

        Bitboard rookMagicMasks[64];
        Bitboard bishopMagicMasks[64];


    };



} // Engine

#endif //UNTITLED_MOVEGENERATOR_H
