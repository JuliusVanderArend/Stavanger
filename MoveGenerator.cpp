//
// Created by Julius on 03/03/2024.
//

#include "MoveGenerator.h"
#include <random>



namespace Engine {
    MoveGenerator::MoveGenerator() {
        for (int i = 0; i < 64; ++i) {
            rookMagicMasks[i] = generateRookMagicMask(i);
            bishopMagicMasks[i] = generateBishopMagicMask(i);
        }

        initalizeMagics();
        std::cout << "Magics Initialized" << std::endl ;
    }


    std::vector<Move> MoveGenerator::generateMoves(Position* position){
        std::vector<Move> generatedMoves; //218 maximum moves from any given position
        generatedMoves.reserve(218);

        for (int i = 0; i < 64; i+=1) {
            Bitboard lut = rookMagicAttacks[i][(position->occupancy & rookMagicMasks[i]).to_ullong() * rookMagics[i].magic
                    >> (64 - rookMagics[i].indexSize)]; //shouldnt be ullong???
            Bitboard gened = floodfillRookAttacks(i, position->occupancy);
            for (int j = 0; j < rookMagicMovesets[i][(position->occupancy & rookMagicMasks[i]).to_ullong() * rookMagics[i].magic
                    >> (64 - rookMagics[i].indexSize)].size(); ++j) {
                printMove(rookMagicMovesets[i][(position->occupancy & rookMagicMasks[i]).to_ullong() * rookMagics[i].magic
                        >> (64 - rookMagics[i].indexSize)][j]);
            }
            std::cout << i <<std::endl;
            if (lut != gened ) {
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
            Bitboard lut = bishopMagicAttacks[i][(position->occupancy & bishopMagicMasks[i]).to_ullong() * bishopMagics[i].magic
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

    void MoveGenerator::generateRookMoves(std::vector<Move>& moves,Position* position,int squareIndex){
        std::vector<Move> rookMoves = rookMagicMovesets[squareIndex][(position->occupancy & rookMagicMasks[squareIndex]).to_ullong() * rookMagics[squareIndex].magic
                >> (64 - rookMagics[squareIndex].indexSize)];

        Bitboard friendlyOccupancy = position->whiteToMove? position->whiteOccupancy : position->blackOccupancy;

        for (int i = 0; i < rookMoves.size(); ++i) { // maybe try performance with std::vector.insert later
            if(rookMoves[i] & (1 <<31)){ // if is capture, check for own captures
                if(!(friendlyOccupancy.to_ullong() & (rookMoves[i]&0x3f))){//if no self capture
                    moves.emplace_back(rookMoves[i]);
                }
            }
        }
    }

    void MoveGenerator::generateBishopMoves(std::vector<Move>& moves,Position* position,int squareIndex){//slow??
        std::vector<Move> bishopMoves = bishopMagicMovesets[squareIndex][(position->occupancy & bishopMagicMasks[squareIndex]).to_ullong() * bishopMagics[squareIndex].magic
                >> (64 - bishopMagics[squareIndex].indexSize)];

        Bitboard friendlyOccupancy = position->whiteToMove? position->whiteOccupancy : position->blackOccupancy;

        for (int i = 0; i < bishopMoves.size(); ++i) { // maybe try performance with std::vector.insert later
            if(bishopMoves[i] & (1 <<31)){ // if is capture, check for own captures
                if(!(friendlyOccupancy.to_ullong() & (bishopMoves[i]&0x3f))){//if no self capture
                    moves.emplace_back(bishopMoves[i]);
                }
            }
        }
    }

    void MoveGenerator::generateQueenMoves(std::vector<Move>& moves,Position* position,int squareIndex){//slow??
        generateBishopMoves(moves,position,squareIndex);
        generateRookMoves(moves,position,squareIndex);
    }

    void MoveGenerator::generateKnightMoves(std::vector<Move>& moves,Position* position,int squareIndex){
        int x = squareIndex % 8;
        int y = squareIndex / 8;
        std::vector<std::pair<int, int>> directions = {{1, 2}, {2, 1}, {2, -1}, {1, -2},
                                                       {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};
        Bitboard friendlyOccupancy = position->whiteToMove? position->whiteOccupancy : position->blackOccupancy;
        for (const auto& dir : directions) {
            int newRank = x + dir.first;
            int newFile = y + dir.second;
            if (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) {
                int newSquareIndex = newRank * 8 + newFile;
                if((position->occupancy & (std::bitset<64>(1) << (newSquareIndex))).none()) { // no capture
                    moves.emplace_back(newSquareIndex | (squareIndex <<6));
                }
                else if((friendlyOccupancy & (std::bitset<64>(1) << (newSquareIndex))).none()){ // no self capture
                    moves.emplace_back(newSquareIndex | (squareIndex <<6) | (1<<14));
                }
            }
        }
    }

    void MoveGenerator::generateKingMoves(std::vector<Move>& moves,Position* position,int squareIndex){
        int x = squareIndex % 8;
        int y = squareIndex / 8;
        std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 1}, {1, 0}, {1, -1},
                                                       {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};
        Bitboard friendlyOccupancy = position->whiteToMove? position->whiteOccupancy : position->blackOccupancy;

        for (const auto& dir : directions) {
            int newRank = x + dir.first;
            int newFile = y + dir.second;
            if (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) {
                int newSquareIndex = newRank * 8 + newFile;
                if((position->occupancy & (std::bitset<64>(1) << (newSquareIndex))).none()) { // no capture
                    moves.emplace_back(newSquareIndex | (squareIndex <<6));
                }
                else if((friendlyOccupancy & (std::bitset<64>(1) << (newSquareIndex))).none()){ // no self capture
                    moves.emplace_back(newSquareIndex | (squareIndex <<6) | (1<<14));
                }
            }
        }
    }

    void MoveGenerator::generatePawnMoves(std::vector<Move>& moves,Position* position,int squareIndex){ // Need to do promotions
        int upOne = position->whiteToMove? 8:-8;
        int upTwo = 2*upOne;

        if((position->occupancy & (std::bitset<64>(1) << (squareIndex+upOne))).none()){//square ahead is empty
            moves.emplace_back((squareIndex+upOne) | (squareIndex <<6));
            if((position->occupancy & (std::bitset<64>(1) << (squareIndex+upTwo))).none()){// two squares ahead is also empty
                int triggerEP = 0;
                if((position->whiteToMove && squareIndex >7 && squareIndex <16) || (!position->whiteToMove && squareIndex >47 && squareIndex <56)){
                    // is a double pawn push off the starting rank
                    triggerEP = 1;
                }
                moves.emplace_back((squareIndex+upTwo) | (squareIndex <<6) | (triggerEP << 12));
            }
        }

        int leftAttack = position->whiteToMove? 7:-9;
        int rightAttack = position->whiteToMove? 9:-7;
        Bitboard enemyOccupancy = position->whiteToMove? position->blackOccupancy : position->whiteOccupancy;

        //left attack
        if(squareIndex%8 > 0 && ((enemyOccupancy | (std::bitset<64>(1) << (position->enPassantTarget)))&(std::bitset<64>(1) << (squareIndex+leftAttack))).any()){
            int code = 4;//only capture
            if(squareIndex+leftAttack == position->enPassantTarget){
                code +=1;//also ep capture
            }
            moves.emplace_back((squareIndex+leftAttack) | (squareIndex <<6) | (code << 14));
        }
        //right attack
        if(squareIndex%8 <7 && ((enemyOccupancy | (std::bitset<64>(1) << (position->enPassantTarget)))&(std::bitset<64>(1) << (squareIndex+rightAttack))).any()){
            int code = 4;//only capture
            if(squareIndex+rightAttack == position->enPassantTarget){
                code +=1;//also ep capture
            }
            moves.emplace_back((squareIndex+rightAttack) | (squareIndex <<6) | (code << 14));
        }
    }




    void MoveGenerator::initalizeMagics() {
        std::random_device rd;
        std::mt19937_64 gen(rd());

        std::uniform_int_distribution<uint64_t> dis;

        int magicsTried = 0;
        for (int i = 0; i < 64; ++i) {
            int shift = -1;
            uint64_t randomMagic;
            while (shift == -1) {
                randomMagic = dis(gen) & dis(gen) & dis(gen);
                shift = validateMagic(true, i, randomMagic);
                magicsTried++;
            }
            std::cout<< std::to_string(i+1)+ " magics found "+std::to_string(magicsTried)+ " magics tried" << std::endl;
            bishopMagics[i] = magicEntry(shift,randomMagic);
        }
        for (int i = 0; i < 64; ++i) {
            int shift = -1;
            uint64_t randomMagic;
            while (shift == -1) {
                randomMagic = dis(gen) * dis(gen) * dis(gen);
                shift = validateMagic(false, i, randomMagic);
                magicsTried++;
            }
            std::cout<< std::to_string(i+65)+ " magics found "+std::to_string(magicsTried)+ " magics tried" << std::endl;
            rookMagics[i] = magicEntry(shift,randomMagic);
        }
    }


    int MoveGenerator::validateMagic(bool isBishop, int squareIndex, uint64_t magic){
        int shift = isBishop ? 5 : 10;
        Bitboard mask = (isBishop) ? bishopMagicMasks[squareIndex] :rookMagicMasks[squareIndex];
        std::vector<Bitboard> blockerConfigs;
        generateAllBlockerConfigs(blockerConfigs,mask);

        std::array<Bitboard, 1 << MAX_MAGIC_BITS>* attackTable = isBishop ? &bishopMagicAttacks[squareIndex] : &rookMagicAttacks[squareIndex];
        std::array<std::vector<Move>,1 << MAX_MAGIC_BITS>* movesetTable = isBishop ? &bishopMagicMovesets[squareIndex] : &rookMagicMovesets[squareIndex];

        while (shift < MAX_MAGIC_BITS){

            for (int i = 0; i < 1 << shift; ++i) {
                (*attackTable)[i] = 0;
            }
            bool collisions = false;

            for (int i = 0; i < (1 << mask.count()); ++i) {

                int index = blockerConfigs[i].to_ullong() * magic >> (64-shift); // what happens when own piece square is included in blockers?

                Bitboard attack = isBishop ? floodfillBishopAttacks(squareIndex, blockerConfigs[i]) : floodfillRookAttacks(squareIndex, blockerConfigs[i]);

                if((*attackTable)[index] == 0){
                    (*attackTable)[index] = attack;
                    (*movesetTable)[index] = attackMapToMoveset(attack, blockerConfigs[i],squareIndex);
                }
                else if((*attackTable)[index] != attack){ //collision
                    shift++;
                    collisions = true;
                    break;
                }
            }

            if(!collisions){
                return shift;
            }
        }
        return -1;
    }


    std::vector<Move> MoveGenerator::attackMapToMoveset(Bitboard attackMap,Bitboard blockers, uint32_t squareIndex){
        std::vector<Move> moves;
        for (uint32_t i = 0; i < 64; ++i) {
            if(attackMap.test(i)){
                uint32_t capture = 0;
                if(blockers.test(i)) {
                    capture = 1;
                }
                moves.emplace_back(i | (squareIndex << 6) | (capture << 14)); //will allow piece to stay in same place? -> YES MUST FIX
            }
        }
        return moves;
    }


    void MoveGenerator::generateAllBlockerConfigs(std::vector<Bitboard>& configs, Bitboard mask){
        int setBits[mask.count()];//move high bit extraction to a lut later?
        int currentBit = 0;

        for (int i = 0; i < 64; ++i) {
            if(mask.test(i)){
                setBits[currentBit] = i;
                currentBit++;
            }
        }

        for (int i = 0; i < (1 << mask.count()); ++i) {
            Bitboard blockers;
            for (int j = 0; j < mask.count(); ++j) {
                if((i >> j) & 1){
                    blockers.set(setBits[j]);
                }
            }
            configs.push_back(blockers);
        }
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

        directionToXYStep(direction,stepX,stepY);

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
        int initialX = x;
        int initialY =y;

        directionToXYStep(direction,stepX,stepY);

        while(x >= 0 && x < 8 && y >=0 && y < 8){
            mask.set(x +y*8);
            if(blockers.test(x+y*8)){
                break;
            }
            x+=stepX;
            y+=stepY;
        }
        mask.reset(initialX + initialY*8);
        return mask;
    }


    void MoveGenerator::directionToXYStep(int direction, int& stepX, int& stepY){
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
    }
} // Engine
