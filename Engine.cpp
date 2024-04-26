//
// Created by Julius on 24/04/2024.
//

#include "Engine.h"

namespace Engine {
    int perftVerbose(Position* position,MoveGenerator* gen, int depth){
        std::vector<Move> moves = gen->generateMoves(position);

        int count = 0;

        for (const auto& move : moves) {
            position->makeMove(move);
            if(!gen->squareIsAttacked(__builtin_ffsll(position->pieceOccupancy[6+(position->whiteToMove?6:0)]),position)){ //king must not be in check REMEBER TO SWAP 0 AND 6
                int subcount = perft(position,gen,depth-1);
                count += subcount;
                printMove(move);
                std::cout << subcount << std::endl;
            }
            position->unMakeMove(move);
        }
        return count;
    }

    int perft(Position* position,MoveGenerator* gen, int depth){
        std::vector<Move> moves = gen->generateMoves(position);
        int count = 0;

        if(depth==0){
            return moves.size();
//            for (const auto& move : moves) {
//                position->makeMove(move);
//                if(!gen->squareIsAttacked(__builtin_ffsll(position->pieceOccupancy[6+(position->whiteToMove?0:6)]),position)){ //king must not be in check REMEBER TO SWAP 0 AND 6
//                    count += 1;
//                }
//                position->unMakeMove(move);
//            }
//            return count;
        }



        for (const auto& move : moves) {
            position->makeMove(move);
            if(!gen->squareIsAttacked(__builtin_ffsll(position->pieceOccupancy[6+(position->whiteToMove?6:0)]),position)){ //king must not be in check
                count += perft(position,gen,depth-1);
            }
            position->unMakeMove(move);
        }


        return count;
    }
}
