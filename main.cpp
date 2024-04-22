



#include <iostream>
#include <chrono>
#include "Position.h"
#include "MoveGenerator.h"
#include "Helper.h"

using namespace Engine;


int main() {
    std::cout << "Hello, World!" << std::endl;

    Position pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
//    pos->whiteToMove = false;
//    pos->debugDraw("occupancy");
//    pos->debugDraw("white");
//    pos->debugDraw("black");

    std::vector<Move> generatedMoves;
    MoveGenerator moveGen;


    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000; ++i) {
       generatedMoves =  moveGen.generateMoves(&pos);
    }
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;

    pos.draw();
    generatedMoves =  moveGen.generateMoves(&pos);
    std::cout << generatedMoves.size() << std::endl;
    for(Move move : generatedMoves){
        printMove(move);
    }
//    pos->makeMove(generatedMoves[0]);
//    pos->draw();

    return 0;
}


