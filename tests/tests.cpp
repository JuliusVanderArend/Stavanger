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


//Move Generation Tests

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

//Position Query tests

TEST(MoveGenerator, squareIsChecked){
    Engine::MoveGenerator gen;
    Engine::Position pos("r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4"); //Symmetrical Italian 3 ply deep

    EXPECT_FALSE(gen.squareIsAttacked(4,&pos)); //king should not be in check

    pos.loadFromFEN("rnb1kbnr/pppp1ppp/8/4p3/4P2q/5P2/PPPP2PP/RNBQKBNR w KQkq - 1 3");// e4,e5,f3,qh4+

    EXPECT_TRUE(gen.squareIsAttacked(4,&pos)); //king should be in check
}


// Position tests

TEST(Position, makeMoveQuiet){
    Engine::Position beforeMove("r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4"); //Symmetrical Italian 3 ply deep
    Engine::Position afterMove("r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPPQPPP/RNB1K2R b KQkq - 5 4"); //Symmetrical Italian 3 ply deep after Qe2

    Move move = 12 | (3<<6); //move Qe2

    beforeMove.makeMove(move);

    EXPECT_EQ(beforeMove.whiteKingsideCastle,afterMove.whiteKingsideCastle);
    EXPECT_EQ(beforeMove.whiteQueensideCastle,afterMove.whiteQueensideCastle);
    EXPECT_EQ(beforeMove.blackKingsideCastle,afterMove.blackKingsideCastle);
    EXPECT_EQ(beforeMove.blackQueensideCastle,afterMove.blackQueensideCastle);

    EXPECT_EQ(beforeMove.enPassantTarget,afterMove.enPassantTarget);
    EXPECT_FALSE(afterMove.whiteToMove);

    EXPECT_EQ(beforeMove.occupancy,afterMove.occupancy);
    EXPECT_EQ(beforeMove.whiteOccupancy,afterMove.whiteOccupancy);
    EXPECT_EQ(beforeMove.blackOccupancy,afterMove.blackOccupancy);
    EXPECT_EQ(*beforeMove.friendlyOccupancy,*afterMove.friendlyOccupancy);
    EXPECT_EQ(*beforeMove.enemyOccupancy,*afterMove.enemyOccupancy);

    for (int i = 0; i < 64; ++i) {
        EXPECT_EQ(beforeMove.pieceNames[i],afterMove.pieceNames[i]) << "different at square " << i;
    }
    for (int i = 0; i < 13; ++i) {
        EXPECT_EQ(beforeMove.pieceOccupancy[i],afterMove.pieceOccupancy[i])<< "different at piece occupancy " << i;
    }
}