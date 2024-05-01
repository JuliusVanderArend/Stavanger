//
// Created by Julius on 24/04/2024.
//

#ifndef UNTITLED_ENGINE_H
#define UNTITLED_ENGINE_H

#include "MoveGenerator.h"
#include "Position.h"
namespace Engine {

    class Stavanger {
    public:
        Stavanger(Position& pos,MoveGenerator& gen);

        Position& pos;
        MoveGenerator& gen;
        int searchDepth =6;

        void makeMove(Move move);
        Move bestMove();

    private:
        static constexpr int PAWN_VALUE = 100;
        static constexpr int KNIGHT_VALUE = 320;
        static constexpr int BISHOP_VALUE = 330;
        static constexpr int ROOK_VALUE = 500;
        static constexpr int QUEEN_VALUE = 900;


        float search(Position& pos, int depth, float alpha, float beta);
        float eval(Position& pos);
    };
    int perft(Position& position,MoveGenerator* gen, int depth,int& nodes);
    int perftVerbose(Position& position,MoveGenerator* gen, int depth);
}



#endif //UNTITLED_ENGINE_H
