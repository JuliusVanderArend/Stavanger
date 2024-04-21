



#include <iostream>
#include <chrono>
#include "Position.h"
#include "MoveGenerator.h"
#include "Helper.h"

using namespace Engine;


int main() {
    std::cout << "Hello, World!" << std::endl;

    Position* pos = new Position();
//    pos->whiteToMove = false;

    pos->draw();
//    pos->debugDraw("occupancy");
//    pos->debugDraw("white");
//    pos->debugDraw("black");

    std::vector<Move> generatedMoves;
    MoveGenerator* moveGen = new MoveGenerator();

    Bitboard test;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100000000; ++i) {
       generatedMoves =  moveGen->generateMoves(pos);
    }
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;

    std::cout << generatedMoves.size() << std::endl;
    for(Move move : generatedMoves){
        printMove(move);
    }

    return 0;
}


