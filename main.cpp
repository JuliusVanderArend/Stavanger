



#include <iostream>
#include "Position.h"
#include "MoveGenerator.h"
#include "Helper.h"

using namespace Engine;


int main() {
    std::cout << "Hello, World!" << std::endl;

    Position* pos = new Position();
    pos->whiteToMove = false;

    pos->draw();
//    pos->debugDraw("occupancy");
//    pos->debugDraw("white");
//    pos->debugDraw("black");

    std::vector<Move> generatedMoves;
    MoveGenerator* moveGen = new MoveGenerator();
    generatedMoves = moveGen->generateMoves(pos);

    std::cout << generatedMoves.size() << std::endl;
    for(Move move : generatedMoves){
        printMove(move);
    }

    return 0;
}


