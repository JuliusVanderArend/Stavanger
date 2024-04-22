//
// Created by Julius on 03/03/2024.
//


#ifndef UNTITLED_MOVEGENERATOR_H
#define UNTITLED_MOVEGENERATOR_H

#include <array>
#include <vector>
#include "Position.h"
#include "Helper.h"

namespace Engine {
    struct magicEntry{
        int indexSize;
        uint64_t magic;

//        magicEntry(int s, uint64_t magic) : indexSize(s), magic(magic) {}
    };

    class MoveGenerator {
    public:
        MoveGenerator();
        std::vector<Move> generateMoves(Position* position);
        static bool squareIsAttacked(int square, Position* position);

    private:
        static const int MAX_MAGIC_BITS = 15;

        static Bitboard generateMagicMaskRay(int direction, int squareIndex);
        static Bitboard generateRookMagicMask(int squareIndex);
        static Bitboard generateBishopMagicMask(int squareIndex);
        static Bitboard floodFill(int direction,  int squareIndex, Bitboard blockers);
        static Bitboard floodfillRookAttacks(int position,Bitboard blockers);
        static Bitboard floodfillBishopAttacks(int position,Bitboard blockers);
        static Bitboard generateKnightAttacks(int squareIndex);
        static Bitboard generateKingAttacks(int squareIndex);
        static Bitboard generatePawnAttacks(int square, bool whiteToMove);

        static void directionToXYStep(int direction, int& stepX, int& stepY);

        static void generateAllBlockerConfigs(std::vector<Bitboard>& configs,Bitboard mask);

        static void initializeMagics();
        static int validateMagic(bool isBishop, int squareIndex, uint64_t magic);

        static std::vector<Move> attackMapToMoveset(Bitboard attackMap,Bitboard blockers, uint32_t squareIndex);

        static void generateMagicMoves(std::vector<Move>& moves,Position* position,Bitboard pieces, std::array<std::array<Bitboard,
                                1 << MAX_MAGIC_BITS>,64>& magicAttacks, std::array<Bitboard,64>& magicMasks, std::array<magicEntry,64> magics);
        static void generateKnightMoves(std::vector<Move>& moves,Position* position,Bitboard knights);
        static void generateKingMoves(std::vector<Move>& moves,Position* position,int square);
        static void generatePawnMoves(std::vector<Move>& moves,Position* position,Bitboard pawns);

        inline static std::array<magicEntry,64> rookMagics;
        inline static std::array<magicEntry,64> bishopMagics;

        inline static std::array<std::array<Bitboard,1 << MAX_MAGIC_BITS>,64> rookMagicAttacks;
        inline static std::array<std::array<Bitboard,1 << MAX_MAGIC_BITS>,64> bishopMagicAttacks; // can be smaller?
        inline static std::array<Bitboard,64> knightAttacks;
        inline static std::array<Bitboard,64> kingAttacks;
        inline static std::array<Bitboard,64> whitePawnAttacks;
        inline static std::array<Bitboard,64> blackPawnAttacks;

        inline static std::array<std::array<std::vector<Move>,1 << MAX_MAGIC_BITS>,64> rookMagicMovesets;
        inline static std::array<std::array<std::vector<Move>,1 << MAX_MAGIC_BITS>,64> bishopMagicMovesets;

        inline static std::array<Bitboard ,64> rookMagicMasks;
        inline static std::array<Bitboard ,64> bishopMagicMasks;
    };



} // Engine

#endif //UNTITLED_MOVEGENERATOR_H
