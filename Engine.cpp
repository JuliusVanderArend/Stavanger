//
// Created by Julius on 24/04/2024.
//

#include "Engine.h"
#include <limits>


namespace Engine {

    Stavanger::Stavanger(Position& position,MoveGenerator& generator): gen(generator), pos(position){
    }

    void Stavanger::makeMove(Move move){
        pos.makeMove(move);
    }

    Move Stavanger::bestMove(){
        std::vector<Move> moves = gen.generateMoves(&pos);
        Move best = 0;
        float bestEval = std::numeric_limits<float>::lowest();

        for (const auto& move : moves) {
            pos.makeMove(move);

            if(!gen.squareIsAttacked(__builtin_ffsll(pos.pieceOccupancy[6+(pos.whiteToMove?0:6)])-1,&pos)){ // if move is legal
                float evaluation = -search(pos,searchDepth,std::numeric_limits<float>::lowest(),std::numeric_limits<float>::max());
                if(evaluation >=  bestEval){
                    best = move;
                    bestEval = evaluation;
                }
            }
            pos.unMakeMove(move);

        }
        return best;
    }





    float Stavanger::search(Position& position, int depth, float alpha, float beta){
        if(depth == 0){
            return eval(position);
        }

        std::vector<Move> moves = gen.generateMoves(&position);

        if(moves.empty()){
            return std::numeric_limits<float>::lowest(); //?
        }


        for (const auto& move : moves) {
            position.makeMove(move);

            if(!gen.squareIsAttacked(__builtin_ffsll(position.pieceOccupancy[6+(position.whiteToMove?0:6)])-1,&position)){ // if move is legal
                float evaluation = -search(position,depth-1,-beta,-alpha);
                position.unMakeMove(move);
                if(evaluation >= beta){
                    return beta;
                }
                alpha = std::max(alpha,evaluation);
            }
            else{
                position.unMakeMove(move);
            }
        }
        return alpha;
    }

    float Stavanger::eval(Position& position){
        float score = 0;
        int toMove = position.whiteToMove? 6:0;

        int num_pawns = __builtin_popcountll(position.pieceOccupancy[1+toMove]);
        int num_knights = __builtin_popcountll(position.pieceOccupancy[2+toMove]);
        int num_bishops = __builtin_popcountll(position.pieceOccupancy[3+toMove]);
        int num_rooks = __builtin_popcountll(position.pieceOccupancy[4+toMove]);
        int num_queens = __builtin_popcountll(position.pieceOccupancy[5+toMove]);

        score += PAWN_VALUE * num_pawns;
        score += KNIGHT_VALUE * num_knights;
        score += BISHOP_VALUE * num_bishops;
        score += ROOK_VALUE * num_rooks;
        score += QUEEN_VALUE * num_queens;

        return score;
    }



    int perftVerbose(Position& position,MoveGenerator* gen, int depth){
        std::vector<Move> moves = gen->generateMoves(&position);
        int count = 0;
        int nodes = 1;

        for (const auto& move : moves) {
            position.makeMove(move);
            if(!gen->squareIsAttacked(__builtin_ffsll(position.pieceOccupancy[6+(position.whiteToMove?0:6)])-1,&position)){ //king must not be in check REMEBER TO SWAP 0 AND 6
                int subcount = perft(position,gen,depth-1,nodes);
                count += subcount;
                printMove(move);
                std::cout << subcount << std::endl;
            }
            position.unMakeMove(move);
        }
        return count;
    }

    int perft(Position& position,MoveGenerator* gen, int depth,int& nodes){
//        if(depth == 0 && nodes == 117661){
//            std::cout <<"ERR"<<std::endl;
//        }
        std::vector<Move> moves = gen->generateMoves(&position);
        int count = 0;
        nodes++;



        if(depth==0){
//            return moves.size();
            for (const auto& move : moves) {
                Position before = position;
                position.makeMove(move);
                if(!gen->squareIsAttacked(__builtin_ffsll(position.pieceOccupancy[6+(position.whiteToMove?0:6)])-1,&position)){ //king must not be in check REMEBER TO SWAP 0 AND 6
                    count += 1;
                }
                position.unMakeMove(move);

                if(!position.equals(before)){
                    std::cout <<"ERR"<<std::endl;
                }
            }
            return count;
        }

        for (const auto& move : moves) {
            Position before = position;
            position.makeMove(move);
            if(!gen->squareIsAttacked(__builtin_ffsll(position.pieceOccupancy[6+(position.whiteToMove?0:6)])-1,&position)){ //king must not be in check
                count += perft(position,gen,depth-1,nodes);
            }
            position.unMakeMove(move);
            if(!position.equals(before)){
                std::cout <<"ERR"<<std::endl;
            }
        }
        return count;
    }
}
