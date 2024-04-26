//
// Created by Julius on 24/04/2024.
//

#ifndef UNTITLED_ENGINE_H
#define UNTITLED_ENGINE_H

#include "MoveGenerator.h"
#include "Position.h"
namespace Engine {
    int perft(Position* position,MoveGenerator* gen, int depth);
    int perftVerbose(Position* position,MoveGenerator* gen, int depth);
}



#endif //UNTITLED_ENGINE_H
