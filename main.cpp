



#include <iostream>
#include "Position.h"
#include "MoveGenerator.h"

using namespace Engine;


int main() {
    std::cout << "Hello, World!" << std::endl;

    Position* pos = new Position();

    int index = pos->getPieceIndex(Piece::WHITE_ROOK,2,2);
//    pos->movePiece(index,2,4);

    pos->draw();
    pos->debugDraw("occupancy");
//    pos->debugDraw("white");
//    pos->debugDraw("black");

    std::vector<Move> generatedMoves;
    MoveGenerator* moveGen = new MoveGenerator();
    generatedMoves = moveGen->generateMoves(pos);

//    for (int i = 0; i < 64; ++i) {
//        std::cout << "================" << std::endl ;
//        pos->drawBitboard(generatedMoves[i]);//move this method to helper;
//    }

    return 0;
}


