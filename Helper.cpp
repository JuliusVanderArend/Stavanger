//
// Created by Julius on 02/03/2024.
//

#include "Position.h" //necessary?
#include "Helper.h"

namespace Engine {
    bool isSliding(Engine::Piece piece){
        return 3 <= abs(piece) &&  abs(piece)<= 5;
    }

} // Helper