//
// Created by Julius on 03/03/2024.
//

#include "MoveGenerator.h"
#include <random>



namespace Engine {
    MoveGenerator::MoveGenerator() {
        for (int i = 0; i < 64; ++i) { // set up lookup tables
            rookMagicMasks[i] = generateRookMagicMask(i);
            bishopMagicMasks[i] = generateBishopMagicMask(i);
            knightAttacks[i] = generateKnightAttacks(i);
            kingAttacks[i] = generateKingAttacks(i);
            whitePawnAttacks[i] = generatePawnAttacks(i,true);
            blackPawnAttacks[i] = generatePawnAttacks(i,false);
        }

        initializeMagics();
        std::cout << "Magics Initialized" << std::endl ;
    }


    std::vector<Move> MoveGenerator::generateMoves(Position* position){
        std::vector<Move> generatedMoves; //218 maximum moves from any given position
        generatedMoves.reserve(218);

        int toMove = position->whiteToMove? 1:-1;
        generateKingMoves(generatedMoves,position,__builtin_ffsll(position->pieceOccupancy[6*toMove+6])-1);
        generatePawnMoves(generatedMoves,position,position->pieceOccupancy[1*toMove+6]);//maybe can just get rid of +6??
        generateKnightMoves(generatedMoves,position,position->pieceOccupancy[2*toMove+6]);
        generateMagicMoves(generatedMoves,position,position->pieceOccupancy[3*toMove+6],bishopMagicAttacks,bishopMagicMasks,bishopMagics); //bishops //ADD OPTIMISATION TO GENERATE ROOK AND BISHOP LIKE QUEEN MOVES WITH ROOKS AND BISHOPS
        generateMagicMoves(generatedMoves,position,position->pieceOccupancy[4*toMove+6],rookMagicAttacks,rookMagicMasks,rookMagics); //rooks
        generateMagicMoves(generatedMoves,position,position->pieceOccupancy[5*toMove+6],bishopMagicAttacks,bishopMagicMasks,bishopMagics); //queens
        generateMagicMoves(generatedMoves,position,position->pieceOccupancy[5*toMove+6],rookMagicAttacks,rookMagicMasks,rookMagics);
        return generatedMoves;
    }

    void MoveGenerator::generateMagicMoves(std::vector<Move>& moves,Position* position,Bitboard pieces, std::array<std::array<Bitboard,1 << MAX_MAGIC_BITS>,64>& magicAttacks, std::array<Bitboard,64>& magicMasks, std::array<magicEntry,64> magics){ //refactor all LUT based pieces to use same function

        Bitboard enemies = position->whiteToMove? position->blackOccupancy : position->whiteOccupancy;

        while(pieces){
            int square = __builtin_ffsll(pieces)-1;
            Bitboard attacks = magicAttacks[square][(position->occupancy & magicMasks[square]) * magics[square].magic >> (64 - magics[square].indexSize)];
            Bitboard captures = attacks & enemies;
            Bitboard quiets = attacks & (~position->occupancy);
            while(quiets){
                int target = __builtin_ffsll(quiets)-1;
                moves.emplace_back(target | (square<<6));
                quiets &= ~(1ULL << target);
            }
            while(captures){
                int target = __builtin_ffsll(captures)-1;
                moves.emplace_back(target | (square<<6) | (4 << 12) | (position->pieceNames[target] << 16));
                captures &= ~(1ULL << target);
            }
            pieces &= ~(1ULL << square);
        }
    }



    void MoveGenerator::generateKnightMoves(std::vector<Move>& moves,Position* position,Bitboard knights){
        Bitboard enemies = position->whiteToMove? position->blackOccupancy : position->whiteOccupancy;

        while(knights){
            int square = __builtin_ffsll(knights)-1;
            Bitboard knightTargets = knightAttacks[square];
            Bitboard quiets = knightTargets & (~position->occupancy);
            Bitboard captures = knightTargets & enemies;
            while(quiets){
                int target = __builtin_ffsll(quiets)-1;
                moves.emplace_back(target | (square<<6));
                quiets &= ~(1ULL << target);
            }
            while(captures){
                int target = __builtin_ffsll(captures)-1;
                moves.emplace_back(target | (square<<6) | (4 << 12) | (position->pieceNames[target] << 16));
                captures &= ~(1ULL << target);
            }
            knights &= ~(1ULL << square);
        }
    }

    Bitboard MoveGenerator::generateKnightAttacks(int square){
        int x = square % 8;
        int y = square / 8;
        Bitboard attacks = 0;
        static constexpr std::array<std::pair<int, int>, 8> directions = {
                {{1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}}
        };
        for (const auto& dir : directions) {
            int newX = x + dir.first;
            int newY = y + dir.second;
            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                int newSquare = newY * 8 + newX;
                attacks |= 1ULL << newSquare;
            }
        }
        return attacks;
    }

    Bitboard MoveGenerator::generateKingAttacks(int square){
        int x = square % 8;
        int y = square / 8;
        Bitboard attacks = 0;
        static constexpr std::array<std::pair<int, int>, 8> directions = {
                {{0, 1}, {1, 1}, {1, 0}, {1, -1},{0, -1}, {-1, -1}, {-1, 0}, {-1, 1}}
        };
        for (const auto& dir : directions) {
            int newX = x + dir.first;
            int newY = y + dir.second;
            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                int newSquare = newY * 8 + newX;
                attacks |= (1ULL << newSquare);
            }
        }
        return attacks;
    }

    Bitboard MoveGenerator::generatePawnAttacks(int square, bool whiteToMove){
        Bitboard attacks = 0;

        // Calculate the pawn attacks
        if (whiteToMove) { // White pawn
            if (square % 8 != 0) // Not in file 'a'
                attacks |= (1ULL << (square + 7)); // Capture to the left
            if (square % 8 != 7) // Not in file 'h'
                attacks |= (1ULL << (square + 9)); // Capture to the right
        } else { // Black pawn
            if (square % 8 != 7) // Not in file 'h'
                attacks |= (1ULL << (square - 7)); // Capture to the right
            if (square % 8 != 0) // Not in file 'a'
                attacks |= (1ULL << (square - 9)); // Capture to the left
        }

        return attacks;
    }


    void MoveGenerator::generateKingMoves(std::vector<Move>& moves,Position* position,int square){ // add castling generation
        Bitboard enemies = position->whiteToMove? position->blackOccupancy : position->whiteOccupancy;

        Bitboard attacks = kingAttacks[square];
        Bitboard quiets = attacks & (~position->occupancy);
        Bitboard captures = attacks & enemies;

        // moves and captures
        while(quiets){
            int target = __builtin_ffsll(quiets)-1;
            moves.emplace_back(target | (square<<6));
            quiets &= ~(1ULL << target);
        }
        while(captures){
            int target = __builtin_ffsll(captures)-1;
            moves.emplace_back(target | (square<<6) | (4 << 12) | (position->pieceNames[target] << 16));
            captures &= ~(1ULL << target);
        }

        // castling
        if((position->whiteToMove && (position->whiteKingsideCastle || position->whiteQueensideCastle)) || (~position->whiteToMove && (position->blackKingsideCastle || position->blackQueensideCastle))){
            const Bitboard kingsideMask = position->whiteToMove? 0x60 : 0x6000000000000000;
            const Bitboard queensideMask = position->whiteToMove? 0xE : 0xE000000000000000;

            if(~kingsideMask & position->occupancy){ //kingside castle
                const int target = position->whiteToMove? 6 : 62;
                moves.emplace_back(target | (square<<6) | (2 << 12));
            }
            if(~queensideMask & position->occupancy){ //queenside castle
                const int target = position->whiteToMove? 2 : 58;
                moves.emplace_back(target | (square<<6) | (3 << 12));
            }
        }
    }

    void MoveGenerator::generatePawnMoves(std::vector<Move>& moves,Position* position,Bitboard pawns){ // Need to do promotions
        const Bitboard notAFile = 0xFEFEFEFEFEFEFEFE;
        const Bitboard notHFile = 0x7F7F7F7F7F7F7F7F;


        int upOne = position->whiteToMove? 8:-8;
        int upLeft = position->whiteToMove? 7:-9;
        int upRight = position->whiteToMove? 9:-7;

        Bitboard doublePushTargetRank = position->whiteToMove? 0x00000000FF000000 : 0x000000FF00000000;
        Bitboard promotableRank = position->whiteToMove? 0x00FF000000000000 : 0x0000000000FF00;
        Bitboard enemies = position->whiteToMove? position->blackOccupancy : position->whiteOccupancy;

        Bitboard promotablePawns = pawns & promotableRank;
        Bitboard unpromotablePawns = pawns & ~promotableRank;

        Bitboard singlePushTargets = unpromotablePawns << upOne &(~position->occupancy);
        Bitboard doublePushTargets = singlePushTargets << upOne & (~position->occupancy) & doublePushTargetRank;

        while(singlePushTargets){ //single pawn push
            int index = __builtin_ffsll(singlePushTargets)-1;
            moves.emplace_back(index | (index-upOne)<<6);
            singlePushTargets &= ~(1ULL << index);
        }

        while(doublePushTargets){ //double pawn push
            int index = __builtin_ffsll(doublePushTargets)-1;
            moves.emplace_back(index | (index-upOne*2)<<6| (1 << 12));
            doublePushTargets &= ~(1ULL << index);
        }

        if(promotablePawns){ //promotions
            Bitboard leftAttackPromo = ((promotablePawns & notAFile) << upLeft) & enemies;
            Bitboard rightAttackPromo = ((promotablePawns & notHFile) << upRight) & enemies;
            Bitboard pushPromo = (promotablePawns << upOne) & (~position->occupancy);

            while(leftAttackPromo){
                int index = __builtin_ffsll(leftAttackPromo)-1;
                moves.emplace_back(index | (index-upLeft)<<6 |  (12 << 12));
                moves.emplace_back(index | (index-upLeft)<<6 |  (13 << 12));
                moves.emplace_back(index | (index-upLeft)<<6 |  (14 << 12));
                moves.emplace_back(index | (index-upLeft)<<6 |  (15 << 12));
                leftAttackPromo &= ~(1ULL << index);
            }
            while(rightAttackPromo){
                int index = __builtin_ffsll(rightAttackPromo)-1;
                moves.emplace_back(index | (index-upRight)<<6 |  (12 << 12));
                moves.emplace_back(index | (index-upRight)<<6 |  (13 << 12));
                moves.emplace_back(index | (index-upRight)<<6 |  (14 << 12));
                moves.emplace_back(index | (index-upRight)<<6 |  (15 << 12));
                rightAttackPromo &= ~(1ULL << index);
            }
            while(pushPromo){
                int index = __builtin_ffsll(pushPromo)-1;
                moves.emplace_back(index | (index-upOne)<<6 |  (8 << 12));
                moves.emplace_back(index | (index-upOne)<<6 |  (9 << 12));
                moves.emplace_back(index | (index-upOne)<<6 |  (10 << 12));
                moves.emplace_back(index | (index-upOne)<<6 |  (11 << 12));
                pushPromo &= ~(1ULL << index);
            }
        }

        Bitboard leftAttack = ((unpromotablePawns & notAFile) << upLeft) & enemies;
        Bitboard rightAttack = ((unpromotablePawns & notHFile) << upRight) & enemies;

        while (leftAttack){
            int index = __builtin_ffsll(leftAttack)-1;
            moves.emplace_back(index | (index-upLeft)<<6 | (4 << 12) | (position->pieceNames[index] << 16));
            leftAttack &= ~(1ULL << index);
        }
        while (rightAttack){
            int index = __builtin_ffsll(rightAttack)-1;
            moves.emplace_back(index | (index-upRight)<<6 | (4 << 12) | (position->pieceNames[index] << 16));
            rightAttack &= ~(1ULL << index);
        }

        int leftEP = position->whiteToMove? -9:9;
        int rightEP = position->whiteToMove? -7:7;

        if(position->enPassantTarget !=0){
            if(position->enPassantTarget % 8 == 0){
                if((1ULL <<(position->enPassantTarget +leftEP)) & unpromotablePawns){
                    moves.emplace_back(position->enPassantTarget | (position->enPassantTarget +leftEP)<<6 | (5 << 12)  | (position->pieceNames[position->enPassantTarget] << 16) );
                }
            }
            else if(position->enPassantTarget % 8 == 7){
                if((1ULL <<(position->enPassantTarget +rightEP)) & unpromotablePawns){
                    moves.emplace_back(position->enPassantTarget | (position->enPassantTarget +rightEP)<<6 | (5 << 12)| (position->pieceNames[position->enPassantTarget] << 16));
                }
            }
            else{
                if((1ULL <<(position->enPassantTarget +leftEP)) & unpromotablePawns){
                    moves.emplace_back(position->enPassantTarget | (position->enPassantTarget +leftEP)<<6 | (5 << 12)| (position->pieceNames[position->enPassantTarget] << 16));
                }
                if((1ULL <<(position->enPassantTarget +rightEP)) & unpromotablePawns){
                    moves.emplace_back(position->enPassantTarget | (position->enPassantTarget +rightEP)<<6 | (5 << 12)| (position->pieceNames[position->enPassantTarget] << 16));
                }
            }
        }
    }





    void MoveGenerator::initializeMagics() {
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
                std::cout << shift << std::endl;
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

            for (int i = 0; i < (1 << std::popcount(mask)); ++i) {

                uint64_t index = blockerConfigs[i] * magic >> (64-shift); // what happens when own piece square is included in blockers?
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

    bool MoveGenerator::squareIsAttacked(int square, Position* position){

        int enemyToMove = position->whiteToMove? -1:1;

        Bitboard enemyPawns = position->pieceOccupancy[1*enemyToMove+6];
        Bitboard enemyKnights   = position->pieceOccupancy[2*enemyToMove+6];
        Bitboard enemyBQ = 0;
        Bitboard enemyRQ = enemyBQ = position->pieceOccupancy[5*enemyToMove+6];
        enemyRQ |= position->pieceOccupancy[4*enemyToMove+6];
        enemyBQ |= position->pieceOccupancy[3*enemyToMove+6];

        Bitboard pawnAttacks = position->whiteToMove? whitePawnAttacks[square] : blackPawnAttacks[square];

        return (pawnAttacks & enemyPawns)
               | (knightAttacks[square] & enemyKnights)
               | (bishopMagicAttacks[square][(position->occupancy & bishopMagicMasks[square]) * bishopMagics[square].magic >> (64 - bishopMagics[square].indexSize)]  & enemyBQ)
               | (rookMagicAttacks[square][(position->occupancy & rookMagicMasks[square]) * rookMagics[square].magic >> (64 - rookMagics[square].indexSize)]  & enemyRQ)
                ;
    }


    std::vector<Move> MoveGenerator::attackMapToMoveset(Bitboard attackMap,Bitboard blockers, uint32_t squareIndex){
        std::vector<Move> moves;
        for (uint32_t i = 0; i < 64; ++i) {
            if(attackMap &(1ULL << i)){
                uint32_t capture = 0;
                if(test(blockers,i)) {
                    capture = 1;
                }
                moves.emplace_back(i | (squareIndex << 6) | (capture << 14)); //will allow piece to stay in same place? -> YES MUST FIX
            }
        }
        return moves;
    }


    void MoveGenerator::generateAllBlockerConfigs(std::vector<Bitboard>& configs, Bitboard mask) {
        std::vector<int> setBits;
        for (int i = 0; i < 64; ++i) {
            if (mask & (1ULL << i)) {
                setBits.push_back(i);
            }
        }

        int numBits = setBits.size();
        for (int i = 0; i < (1 << numBits); ++i) {
            Bitboard blockers = 0ULL;
            for (int j = 0; j < numBits; ++j) {
                if (i & (1ULL << j)) {
                    blockers |= (1ULL << setBits[j]);
                }
            }
            configs.push_back(blockers);
        }
    }


    Bitboard MoveGenerator::floodfillRookAttacks(int position,Bitboard blockers){
        Bitboard moves = 0;
        moves |= floodFill(0,position,blockers);
        moves |= floodFill(2,position,blockers);
        moves |= floodFill(4,position,blockers);
        moves |= floodFill(6,position,blockers);

        return moves;
    }


    Bitboard MoveGenerator::floodfillBishopAttacks(int position,Bitboard blockers){
        Bitboard moves = 0;
        moves |= floodFill(1,position,blockers);
        moves |= floodFill(3,position,blockers);
        moves |= floodFill(5,position,blockers);
        moves |= floodFill(7,position,blockers);

        return moves;
    }


    Bitboard MoveGenerator::generateRookMagicMask(int squareIndex){
        Bitboard mask = 0;
        mask |= generateMagicMaskRay(0,squareIndex);
        mask |= generateMagicMaskRay(2,squareIndex);
        mask |= generateMagicMaskRay(4,squareIndex);
        mask |= generateMagicMaskRay(6,squareIndex);
        return mask;
    }


    Bitboard MoveGenerator::generateBishopMagicMask(int squareIndex){
        Bitboard mask = 0;
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
            set(mask,x + y * 8);
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
        clear(mask,squareIndex); //??? how should own square occupancy be considered?
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
//        blockers.reset(initialX + initialY*8); //??? IS THIS RIGHT?
        while(x >= 0 && x < 8 && y >=0 && y < 8){
            set(mask,x +y*8);
            if(test(blockers,x+y*8)){
                break;
            }
            x+=stepX;
            y+=stepY;
//            drawBitboard(mask);
        }
        clear(mask,initialX + initialY*8);
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
