//
// Created by Julius on 03/03/2024.
//

#include "MoveGenerator.h"


namespace Engine {

    MoveGenerator::MoveGenerator() {
        // initialize mask LUTs
        for (int i = 0; i < 8; ++i) {//iterate over each direction
            for (int j = 0; j < 63; ++j) {//iterate over each square
                Bitboard piece;
                piece.set(i);
                rayMaskLookup[i][j] = generateDirectionMask(i,piece);
            }
        }

        for (int i = 0; i < 8; ++i) {//iterate over each direction
            for (int j = 0; j < 63; ++j) {//iterate over each square
                Bitboard piece;
                piece.set(i);
                for (int k = 0; k < 63; ++k) {// iterate over blocker arrangements 
                    rayOcclusionLookup[i][j][k] = floodFill(i,piece,);
                }
            }
        }
    }

    Bitboard * MoveGenerator::generateMoves(Position* position){
        Bitboard * generatedMoves = new Bitboard [128];
        int moveIndex = 0;
        for (int i = 0; i < position->numPieces; ++i) {
            if(isSliding(position->pieceNames[i])){
//                generatedMoves[moveIndex] = generateSlidingMoves(position->pieces[i],position->occupancy,
//                                                                 generateDirectionMask(0,position->pieces[i]));
                generatedMoves[moveIndex] = generateDirectionMask(0,position->pieces[i]);
                moveIndex++;
            }
        }


        return  generatedMoves;
    }

    // generates legal moves for a sliding piece using obstruction difference method
    Bitboard MoveGenerator::generateSlidingMoves(Bitboard piece, Bitboard blockers, Bitboard directionMask) {
        Bitboard attacks = 0;
        Bitboard directionAttacks = piece;

        // Generate moves along the specified direction until blocked
        while (directionAttacks.any()) {
            attacks |= directionAttacks;
            blockers |= directionAttacks;
            directionAttacks = (directionAttacks << directionMask.count()) & ~blockers & directionMask;
        }
        return attacks;
    }

    Bitboard MoveGenerator::generateDirectionMask(int direction, Bitboard piece) {
        Bitboard mask = 0;
        int squareIndex = piece._Find_first();
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
            x+=stepX;
            y+=stepY;
        }
        return mask;
    }

    Bitboard MoveGenerator::floodFill(int direction, Bitboard piece, Bitboard blockers) { //refactor this and generateDirectionMask to use same code
        Bitboard mask = 0;
        int squareIndex = piece._Find_first();
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
            if(blockers.test(x+y*8)){
                break;
            }
            mask.set(x +y*8);
            x+=stepX;
            y+=stepY;
        }
        return mask;
    }

} // Engine