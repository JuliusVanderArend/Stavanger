//
// Created by Julius on 03/03/2024.
//

#include "MoveGenerator.h"
#include <random>


namespace Engine {

    void MoveGenerator::initalizeMagics() {
        std::random_device rd;
        std::mt19937_64 gen(rd());

        std::uniform_int_distribution<uint64_t> dis;

        for (int i = 0; i < 64; ++i) {
            int shift = -1;
            uint64_t randomMagic;
            while (shift == -1) {
                randomMagic = dis(gen) & dis(gen) & dis(gen);//2413929401352716288;
                shift = validateMagic(true, i, randomMagic,bishopMagicLUT[i],bishopMagicMovesets[i]);
            }
            bishopMagics[i] = magicEntry(shift,randomMagic);

        }
        for (int i = 0; i < 64; ++i) {
            int shift = -1;
            uint64_t randomMagic;
            while (shift == -1) {
                randomMagic = dis(gen) * dis(gen) * dis(gen);
                shift = validateMagic(false, i, randomMagic,rookMagicLUT[i],rookMagicMovesets[i]);
            }
            rookMagics[i] = magicEntry(shift,randomMagic);
        }
    }

    int MoveGenerator::validateMagic(bool isBishop, int squareIndex, uint64_t magic, Bitboard*& table, uint32_t**& movesetTable){
        int shift = 10;
        Bitboard mask = (isBishop) ? bishopMagicMasks[squareIndex] :rookMagicMasks[squareIndex];
        int setBits[mask.count()];//move this to a lut later
        int currentBit = 0;

        for (int i = 0; i < 64; ++i) {
            if(mask.test(i)){
                setBits[currentBit] = i;
                currentBit++;
            }
        }

        table = new Bitboard[1 << 17];

        movesetTable = new uint32_t *[1 << 17];
        for (int i = 0; i < 1 << 17; ++i) {
            movesetTable[i] = new uint32_t[32];
        }


        while (shift < 17){

            for (int i = 0; i < 1 << shift; ++i) {
                table[i] = 0;
            }
            bool collisions = false;

            for (int i = 0; i < (1 << mask.count()); ++i) {
                Bitboard blockers;

                for (int j = 0; j < mask.count(); ++j) {
                    if((i >> j) & 1){
                        blockers.set(setBits[j]);
                    }
                }

                int index = blockers.to_ullong() * magic >> (64-shift); // what happens when own piece square is included in blockers?

                if(table[index] == 0){
                    Bitboard attackMap = isBishop ? floodfillBishopAttacks(squareIndex,blockers) : floodfillRookAttacks(squareIndex,blockers);
                    table[index] = attackMap;
                    attackMapToMoveset(attackMap,squareIndex,movesetTable[index]);
                }
                else if(table[index] != (isBishop ? floodfillBishopAttacks(squareIndex,blockers) : floodfillRookAttacks(squareIndex,blockers))){ //collision
                    shift++;
                    collisions = true;
                    break;
                }
            }

            if(!collisions){
                return shift;
            }
        }
        delete[] table;
        for (int i = 0; i < 1 << 17; ++i) {
            delete[] movesetTable[i];
        }
        delete[] movesetTable;
        return -1;

    }

    void MoveGenerator::attackMapToMoveset(Bitboard attackMap, uint32_t squareIndex, uint32_t*& moves){
        int moveIndex = 0;
        for (uint32_t i = 0; i < 64; ++i) {
            if(attackMap.test(i)){
                moves[moveIndex] = squareIndex | (i << 6); //will allow piece to stay in same place?
                moveIndex++;
            }
        }
    }


    MoveGenerator::MoveGenerator() {
        for (int i = 0; i < 64; ++i) {
            rookMagicMasks[i] = generateRookMagicMask(i);
            bishopMagicMasks[i] = generateBishopMagicMask(i);
        }



        initalizeMagics();
        std::cout << "Magics Initialized" << std::endl ;
    }

    Bitboard* MoveGenerator::generateMoves(Position* position){
        Bitboard generatedMoves[218]; //218 maximum moves from any given position
        for (int i = 0; i < 64; i+=1) {
            Bitboard lut = rookMagicLUT[i][(position->occupancy & rookMagicMasks[i]).to_ullong() * rookMagics[i].magic
                    >> (64 - rookMagics[i].indexSize)]; //shouldnt be ullong???
            Bitboard gened = floodfillRookAttacks(i, position->occupancy);
            for (int j = 0; j < 32; ++j) {
                printMove(rookMagicMovesets[i][(position->occupancy & rookMagicMasks[i]).to_ullong() * rookMagics[i].magic
                        >> (64 - rookMagics[i].indexSize)][j]);
            }
            if (lut != gened) {
                std::cout << "................." << std::endl;
                position->drawBitboard(position->occupancy & rookMagicMasks[i]);
                std::cout << "================" << std::endl;
                position->drawBitboard(lut);
                std::cout << "================" << std::endl;
                position->drawBitboard(gened);
                std::cout << "ERROR" << std::endl;
            }
        }

        for (int i = 0; i < 64; i+=1) {
            Bitboard lut = bishopMagicLUT[i][(position->occupancy & bishopMagicMasks[i]).to_ullong() * bishopMagics[i].magic
                    >> (64 - bishopMagics[i].indexSize)]; //shouldnt be ullong???
            Bitboard gened = floodfillBishopAttacks(i, position->occupancy);
            if (lut != gened) {
                std::cout << "................." << std::endl;
                position->drawBitboard(position->occupancy & bishopMagicMasks[i]);
                std::cout << "================" << std::endl;
                position->drawBitboard(lut);
                std::cout << "================" << std::endl;
                position->drawBitboard(gened);
                std::cout << "ERROR" << std::endl;
            }
        }
        return generatedMoves;
    }

    Bitboard MoveGenerator::generateRookMagicMask(int squareIndex){
        Bitboard mask;
        mask |= generateMagicMaskRay(0,squareIndex);
        mask |= generateMagicMaskRay(2,squareIndex);
        mask |= generateMagicMaskRay(4,squareIndex);
        mask |= generateMagicMaskRay(6,squareIndex);
        return mask;
    }

    Bitboard MoveGenerator::generateBishopMagicMask(int squareIndex){
        Bitboard mask;
        mask |= generateMagicMaskRay(1,squareIndex);
        mask |= generateMagicMaskRay(3,squareIndex);
        mask |= generateMagicMaskRay(5,squareIndex);
        mask |= generateMagicMaskRay(7,squareIndex);
        return mask;
    }

    Bitboard MoveGenerator::generateMagicMaskRay(int direction, int squareIndex) {
        Bitboard mask = 0;
        int stepX = 0;
        int stepY = 0;
        int x = squareIndex%8;
        int y = squareIndex/8;
        int initialX = x;
        int initialY = y;
        // Generate masks for each direction
        switch (direction) {
            case 0:  // North
                stepY = 1;
                break;
            case 1:  // Northeast
                stepX = 1;
                stepY = 1;
                break;
            case 2:  // East
                stepX = 1;
                break;
            case 3:  // Southeast
                stepX = 1;
                stepY = -1;
                break;
            case 4:  // South
                stepY = -1;
                break;
            case 5:  // Southwest
                stepX = -1;
                stepY = -1;
                break;
            case 6:  // West
                stepX = -1;
                break;
            case 7:  // Northwest
                stepX = -1;
                stepY = 1;
                break;
            default:
                break;
        }
        while (x >= 0 && x < 8 && y >=0 && y < 8) {
            mask.set(x + y * 8);
            x += stepX;
            y += stepY;
        }
        if(initialX !=0){ //maybe move this masking to subsequent functions
            mask &= 0xFEFEFEFEFEFEFEFE;
        }
        if(initialX !=7){
            mask &= 0x7F7F7F7F7F7F7F7F;
        }
        if(initialY !=0){
            mask &= 0xFFFFFFFFFFFFFF00;
        }
        if(initialY !=7){
            mask &= 0x00FFFFFFFFFFFFFF;
        }
//        mask.reset(initialX+initialY*8); ??? how should own square occupancy be considered?


        return mask;
    }

    Bitboard MoveGenerator::floodFill(int direction, int squareIndex, Bitboard blockers) { //refactor this and generateDirectionMask to use same code
        Bitboard mask = 0;
        int stepX = 0;
        int stepY = 0;
        int x = squareIndex%8;
        int y = squareIndex/8;
        // Generate masks for each direction
        switch (direction) {
            case 0:  // North
                stepY = 1;
                break;
            case 1:  // Northeast
                stepX = 1;
                stepY = 1;
                break;
            case 2:  // East
                stepX = 1;
                break;
            case 3:  // Southeast
                stepX = 1;
                stepY = -1;
                break;
            case 4:  // South
                stepY = -1;
                break;
            case 5:  // Southwest
                stepX = -1;
                stepY = -1;
                break;
            case 6:  // West
                stepX = -1;
                break;
            case 7:  // Northwest
                stepX = -1;
                stepY = 1;
                break;
            default:
                break;
        }
        while(x >= 0 && x < 8 && y >=0 && y < 8){
            mask.set(x +y*8);
            if(blockers.test(x+y*8)){
                break;
            }
            x+=stepX;
            y+=stepY;
        }
        return mask;
    }

    Bitboard MoveGenerator::floodfillRookAttacks(int position,Bitboard blockers){
        Bitboard moves;
        moves |= floodFill(0,position,blockers);
        moves |= floodFill(2,position,blockers);
        moves |= floodFill(4,position,blockers);
        moves |= floodFill(6,position,blockers);

        return moves;
    }

    Bitboard MoveGenerator::floodfillBishopAttacks(int position,Bitboard blockers){
        Bitboard moves;
        moves |= floodFill(1,position,blockers);
        moves |= floodFill(3,position,blockers);
        moves |= floodFill(5,position,blockers);
        moves |= floodFill(7,position,blockers);

        return moves;
    }

} // Engine