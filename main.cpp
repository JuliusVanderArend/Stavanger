



#include <iostream>
#include <chrono>
#include "Position.h"
#include "MoveGenerator.h"
#include "Helper.h"
#include "Engine.h"


using namespace Engine;


int main() {
    std::cout << "Hello, World!" << std::endl;

    Position pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    MoveGenerator moveGen;

    Stavanger engine(pos,moveGen);

    while(true){
        engine.pos.draw();

        std::cout << "Enter your move : ";
        std::string input;
        std::cin >> input;


        Move response = engine.bestMove();
        std::cout<< "+++++++++++" <<std::endl;
        printMove(response);
        engine.makeMove(response);
    }


//    pos->makeMove(generatedMoves[0]);
//    pos->draw();

    return 0;
}


