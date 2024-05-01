



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

        auto tryMove = toMove(input);

        std::vector<Move> moves = moveGen.generateMoves(&engine.pos);

        for(const auto& move : moves){
            printMove(move);
            if(((move >> 6) & 0x3f) ==  std::get<0>(tryMove) && (move & 0x3f) == std::get<1>(tryMove) ){//broken promo
                engine.makeMove(move);
                std::cout<< "+++++++++++" <<std::endl;
                printMove(move);
                break;
            }
        }

        Move response = engine.bestMove();
        std::cout<< "+++++++++++" <<std::endl;
        printMove(response);
        engine.makeMove(response);
    }


//    pos->makeMove(generatedMoves[0]);
//    pos->draw();

    return 0;
}


