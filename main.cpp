



#include <iostream>
#include "Position.h"
#include "MoveGenerator.h"

using namespace Engine;


int main() {
    std::cout << "Hello, World!" << std::endl;

    Position* pos = new Position();

    int index = pos->getPieceIndex(Piece::WHITE_ROOK,2,2);
    pos->movePiece(index,2,4);

    pos->draw();
    pos->debugDrawOccupancy();

    Position* generatedMoves;
    MoveGenerator* moveGen = new MoveGenerator();
    generatedMoves = moveGen->generateMoves(pos);

    return 0;
}


