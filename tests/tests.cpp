//
// Created by Julius on 22/04/2024.
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../Position.h"
#include "../MoveGenerator.h"
#include "../Helper.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

TEST(MoveGenerator, generateMovesStartingPos){
    Engine::MoveGenerator gen;
    Engine::Position pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"); //starting position

    std::vector<Move> generatedMoves = gen.generateMoves(&pos);

    EXPECT_EQ(generatedMoves.size(),20);
}

TEST(MoveGenerator, generateMovesCPWPos5){
    Engine::MoveGenerator gen;
    Engine::Position pos("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8 "); //Position 5 from https://www.chessprogramming.org/Perft_Results

    std::vector<Move> generatedMoves = gen.generateMoves(&pos);

    EXPECT_EQ(generatedMoves.size(),44);
}


TEST(MoveGenerator, squareIsChecked){
    Engine::MoveGenerator gen;
    Engine::Position pos("r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4"); //Symmetrical Italian 3 ply deep

    EXPECT_EQ(gen.squareIsAttacked(4,&pos),false); //king should not be in check

    pos.loadFromFEN("rnb1kbnr/pppp1ppp/8/4p3/4P2q/5P2/PPPP2PP/RNBQKBNR w KQkq - 1 3");// e4,e5,f3,qh4+

    EXPECT_EQ(gen.squareIsAttacked(4,&pos),true); //king should be in check
}