//
// Created by Julius on 03/03/2024.
//

#include "MoveGenerator.h"


namespace Engine {

    Position* MoveGenerator::generateMoves(Position* position){
        Position* generatedMoves = new Position[128];
        for (int i = 0; i < position->numPieces; ++i) {
            if(isSliding(position->pieceNames[i])){
                std::cout << position->pieceNames[i] << std::endl;
            }
        }
        return  generatedMoves;
    }
} // Engine