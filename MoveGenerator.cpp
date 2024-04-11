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
//        for (int i = 0; i < position->numPieces; ++i) {
//            if((position->pieceNames[i] == Piece::WHITE_PAWN && position->whiteToMove) || (position->pieceNames[i] == Piece::BLACK_PAWN && !position->whiteToMove)) {
//                generatePawnMoves(generatedMoves,position,position->pieces[i]._Find_first());//maybe store square index of each peice in position so find first is not needed?
//            }
//            else if((position->pieceNames[i] == Piece::WHITE_KNIGHT && position->whiteToMove) || (position->pieceNames[i] == Piece::BLACK_KNIGHT && !position->whiteToMove)){
//                generateKnightMoves(generatedMoves,position,position->pieces[i]._Find_first());
//            }
//            else if((position->pieceNames[i] == Piece::WHITE_BISHOP && position->whiteToMove) || (position->pieceNames[i] == Piece::BLACK_BISHOP && !position->whiteToMove)){
//                generateBishopMoves(generatedMoves,position,position->pieces[i]._Find_first());
//            }
//            else if((position->pieceNames[i] == Piece::WHITE_ROOK && position->whiteToMove) || (position->pieceNames[i] == Piece::BLACK_ROOK && !position->whiteToMove)){
//                generateRookMoves(generatedMoves,position,position->pieces[i]._Find_first());
//            }
//            else if((position->pieceNames[i] == Piece::WHITE_QUEEN && position->whiteToMove) || (position->pieceNames[i] == Piece::BLACK_KING && !position->whiteToMove)){
//                generateQueenMoves(generatedMoves,position,position->pieces[i]._Find_first());
//            }
//            else if((position->pieceNames[i] == Piece::WHITE_KING && position->whiteToMove) || (position->pieceNames[i] == Piece::BLACK_KING && !position->whiteToMove)){
//                generateKingMoves(generatedMoves,position,position->pieces[i]._Find_first());
//            }
//        }
        int toMove = position->whiteToMove? 1:-1;
        generateKingMoves(generatedMoves,position,__builtin_ffsll(position->pieceOccupancy[6*toMove]));
        generatePawnMoves(generatedMoves,position,__builtin_ffsll(position->pieceOccupancy[1*toMove]));

        while(position->pieceOccupancy[offset+1])


        return generatedMoves;
    }

    void MoveGenerator::generateRookMoves(std::vector<Move>& moves,Position* position,int squareIndex){
        std::vector<Move>& rookMoves = rookMagicMovesets[squareIndex][(position->occupancy & rookMagicMasks[squareIndex]).to_ullong() * rookMagics[squareIndex].magic >> (64 - rookMagics[squareIndex].indexSize)];

        Bitboard friendlyOccupancy = position->whiteToMove? position->whiteOccupancy : position->blackOccupancy;

        for (int i = 0; i < rookMoves.size(); ++i) { // maybe try performance with std::vector.insert later
            if(rookMoves[i] & (1 <<14)){ // if is capture, check for own captures
                if((friendlyOccupancy & (std::bitset<64>(1) <<(rookMoves[i]&0x3f))).none()){//if no self capture
                    moves.emplace_back(rookMoves[i]);
                }
            }
            else{
                moves.emplace_back(rookMoves[i]);
            }
        }
    }

    void MoveGenerator::generateBishopMoves(std::vector<Move>& moves,Position* position,int squareIndex){//slow??
        std::vector<Move>& bishopMoves = bishopMagicMovesets[squareIndex][(position->occupancy & bishopMagicMasks[squareIndex]).to_ullong() * bishopMagics[squareIndex].magic
                >> (64 - bishopMagics[squareIndex].indexSize)];

        Bitboard friendlyOccupancy = position->whiteToMove? position->whiteOccupancy : position->blackOccupancy;

        for (int i = 0; i < bishopMoves.size(); ++i) { // maybe try performance with std::vector.insert later
            if(bishopMoves[i] & (1 <<14)){ // if is capture, check for own captures
                if((friendlyOccupancy & (std::bitset<64>(1) <<(bishopMoves[i]&0x3f))).none()){//if no self capture
                    moves.emplace_back(bishopMoves[i]);
                }
            }
            else{
                moves.emplace_back(bishopMoves[i]);
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
        static constexpr std::array<std::pair<int, int>, 8> directions = {
                {{1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}}
        };
        Bitboard friendlyOccupancy = position->whiteToMove? position->whiteOccupancy : position->blackOccupancy;
        for (const auto& dir : directions) {
            int newX = x + dir.first;
            int newY = y + dir.second;
            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                int newSquareIndex = newY * 8 + newX;
                if((position->occupancy & (std::bitset<64>(1) << (newSquareIndex))).none()) { // no capture
                    moves.emplace_back(newSquareIndex | (squareIndex <<6));
                }
                else if((friendlyOccupancy & (std::bitset<64>(1) << (newSquareIndex))).none()){ // no self capture
                    moves.emplace_back(newSquareIndex | (squareIndex <<6) | (1<<14));
                }
            }
        }
    }

    void MoveGenerator::generateKingMoves(std::vector<Move>& moves,Position* position,int squareIndex){ // add castling generation
        int x = squareIndex % 8;
        int y = squareIndex / 8;
        static constexpr std::array<std::pair<int, int>, 8> directions = {
                {{0, 1}, {1, 1}, {1, 0}, {1, -1},{0, -1}, {-1, -1}, {-1, 0}, {-1, 1}}
        };
        Bitboard friendlyOccupancy = position->whiteToMove? position->whiteOccupancy : position->blackOccupancy;

        for (const auto& dir : directions) {
            int newX = x + dir.first;
            int newY = y + dir.second;
            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                int newSquareIndex = newY * 8 + newX;
                if((position->occupancy & (std::bitset<64>(1) << (newSquareIndex))).none()) { // no capture
                    moves.emplace_back(newSquareIndex | (squareIndex <<6));
                }
                else if((friendlyOccupancy & (std::bitset<64>(1) << (newSquareIndex))).none()){ // no self capture
                    moves.emplace_back(newSquareIndex | (squareIndex <<6) | (1<<14));
                }
            }
        }

        // castling
        if(position->whiteToMove){
            if(position-> whiteKingsideCastle){
                if((position->occupancy & Bitboard(0x60)).none()){
                    moves.emplace_back(6 | (squareIndex <<6) | (2<<12));
                }
            }
            if(position-> whiteQueensideCastle){
                if((position->occupancy & Bitboard(0xE)).none()){
                    moves.emplace_back(2 | (squareIndex <<6) | (3<<12));
                }
            }
        }
        else{
            if(position-> blackKingsideCastle){
                if((position->occupancy & Bitboard(0x6000000000000000)).none()){
                    moves.emplace_back(62 | (squareIndex <<6) | (2<<12));
                }
            }
            if(position-> blackQueensideCastle){
                if((position->occupancy & Bitboard(0xE00000000000000)).none()){
                    moves.emplace_back(58 | (squareIndex <<6) | (3<<12));
                }
            }
        }
    }

    void MoveGenerator::generatePawnMoves(std::vector<Move>& moves,Position* position,Bitboard pawns){ // Need to do promotions
        int upOne = position->whiteToMove? 8:-8;
        Bitboard doublePushTargetRank = position->whiteToMove? 0x00000000FF000000 : 0x000000FF00000000;

        Bitboard promotableRank = position->whiteToMove? 0x00FF000000000000 : 0x0000000000FF00;
        Bitboard promotablePawns = pawns & promotableRank;
        Bitboard unpromotablePawns = pawns & ~promotableRank;

        Bitboard singlePushTargets = unpromotablePawns << upOne &(~position->occupancy);
        Bitboard doublePushTargets = singlePushTargets << upOne & (~position->occupancy) & doublePushTargetRank;
        Bitboard promotionTargets = promotablePawns << upOne &(~position->occupancy);
 v
        while(singlePushTargets){
            int index = __builtinFFSL(singlePushTargets);
            moves.emplaceBack(index | (index-upOne)<<6);
            singlePushTargets &= ~(1ULL << index);
        }
        while(doublePushTargets){
            int index = __builtinFFSL(doublePushTargets);
            moves.emplaceBack(index | (index-upOne*2)<<6| (1 << 12));
            doublePushTargets &= ~(1ULL << index);
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
        mask.reset(squareIndex); //??? how should own square occupancy be considered?
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
