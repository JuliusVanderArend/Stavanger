//
// Created by Julius on 03/03/2024.
//


#ifndef UNTITLED_MOVEGENERATOR_H
#define UNTITLED_MOVEGENERATOR_H

#include "Position.h"


namespace Engine {

    class MoveGenerator {
    public:
        MoveGenerator();
        Bitboard * generateMoves(Position* position);

    private:
        Bitboard generateSlidingMoves(Bitboard piece, Bitboard blockers, Bitboard directionMask);
        Bitboard generateDirectionMask(int direction, Bitboard piece);
        Bitboard floodFill(int direction, Bitboard piece, Bitboard blockers);

        // sliding piece move generation lookup tables for each direction, indexed as [direction(N->W)][square][blocker arrangement]

        Bitboard rayOcclusionLookup[8][64][64];

        Bitboard  rayMaskLookup[8][64];



    };

} // Engine

#endif //UNTITLED_MOVEGENERATOR_H
