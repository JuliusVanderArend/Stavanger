//
// Created by Julius on 22/04/2024.
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../Position.h"
#include "../MoveGenerator.h"
#include "../Helper.h"
#include "../Engine.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}


class MoveGenerator : public ::testing::Test { // move generator fixture
protected:
    static void SetUpTestSuite() {
        // Initialize your move generator here
        gen = new Engine::MoveGenerator();
    }

    static void TearDownTestSuite() {
        // Clean up after each test if needed
        delete gen;
    }

    static Engine::MoveGenerator* gen; // share same move generator for all tests
};

Engine::MoveGenerator* MoveGenerator::gen = nullptr;


void expectPositionsEqual(const Engine::Position& beforeMove, const Engine::Position& afterMove){ //checks if two positions are equal
    EXPECT_EQ(beforeMove.whiteKingsideCastle,afterMove.whiteKingsideCastle);
    EXPECT_EQ(beforeMove.whiteQueensideCastle,afterMove.whiteQueensideCastle);
    EXPECT_EQ(beforeMove.blackKingsideCastle,afterMove.blackKingsideCastle);
    EXPECT_EQ(beforeMove.blackQueensideCastle,afterMove.blackQueensideCastle);

    EXPECT_EQ(beforeMove.enPassantTarget,afterMove.enPassantTarget);

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

    EXPECT_EQ(beforeMove.whiteToMove,afterMove.whiteToMove);

}


//Move Generation Tests

TEST_F(MoveGenerator, generateMovesStartingPos){
    Engine::Position pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"); //starting position

    std::vector<Move> generatedMoves = gen->generateMoves(&pos);

    EXPECT_EQ(generatedMoves.size(),20);
}

TEST_F(MoveGenerator, generateMovesBlackE4){
    Engine::Position pos("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1"); //e4, test movegen for black

    std::vector<Move> generatedMoves = gen->generateMoves(&pos);

    EXPECT_EQ(generatedMoves.size(),20);
}

TEST_F(MoveGenerator, generateMovesCPWPos5){
    Engine::Position pos("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8 "); //Position 5 from https://www.chessprogramming.org/Perft_Results

    std::vector<Move> generatedMoves = gen->generateMoves(&pos);

    EXPECT_EQ(generatedMoves.size(),44);
}

//Position Query tests

TEST_F(MoveGenerator, squareIsChecked){
    Engine::Position pos("r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4"); //Symmetrical Italian 3 ply deep

    EXPECT_TRUE(gen->squareIsAttacked(60,&pos));

    pos.loadFromFEN("rnbqkbnr/pppp2pp/5p2/4p2Q/4P3/3P4/PPP2PPP/RNB1KBNR w KQkq - 1 3"); //illegal position to trip check detection

    EXPECT_FALSE(gen->squareIsAttacked(60,&pos));
}


// Position tests

// Make Move Tests



TEST(Position, makeMoveCapture){
    Engine::Position beforeMove("r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4"); //Symmetrical Italian 3 ply deep
    Engine::Position afterMove("r1bqk1nr/pppp1ppp/2n5/2b1N3/2B1P3/8/PPPP1PPP/RNBQK2R b KQkq - 0 4"); //Symmetrical Italian 3 ply deep after nxe5

    Move move = 36 | (21<<6) |  (4 << 12) | (1 << 16); //move nxe5

    beforeMove.makeMove(move);

    expectPositionsEqual(beforeMove,afterMove);
}

TEST(Position, makeMoveEP){
    Engine::Position beforeMove("rnbqkbnr/ppp2ppp/3p4/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6 0 3"); //d4d6d5e5
    Engine::Position afterMove("rnbqkbnr/ppp2ppp/3pP3/8/8/8/PPP1PPPP/RNBQKBNR b KQkq - 0 3"); //d4d6d5e5 after dxe6

    Move move = 44 | (35<<6) | (5 << 12)  | (1 << 16); //move dxe6

    beforeMove.makeMove(move);

    expectPositionsEqual(beforeMove,afterMove);
}

TEST(Position, makeMoveKingsideCastle){
    Engine::Position beforeMove("r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4"); //Symmetrical Italian 3 ply deep
    Engine::Position afterMove("r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 5 4"); //Symmetrical Italian 3 ply deep after O-O

    Move move = 6 | (4<<6) | (2 << 12); //move O-O

    beforeMove.makeMove(move);

    expectPositionsEqual(beforeMove,afterMove);
}

TEST(Position, makeMoveQueensideCastle){
    Engine::Position beforeMove("rnb1kb1r/ppp2ppp/3q1n2/3pp3/3P1B2/2NQ4/PPP1PPPP/R3KBNR w KQkq - 2 5"); //d4d5Qd3Qd6Bf4e5Nc3Nf6
    Engine::Position afterMove("rnb1kb1r/ppp2ppp/3q1n2/3pp3/3P1B2/2NQ4/PPP1PPPP/2KR1BNR b kq - 3 5"); //d4d5Qd3Qd6Bf4e5Nc3Nf6 after O-O-O

    Move move = 2 | (4<<6) | (3 << 12); //move O-O-O

    beforeMove.makeMove(move);

    expectPositionsEqual(beforeMove,afterMove);
}


TEST(Position, makeMovePromote){
    Engine::Position beforeMove("3r4/1PPPPPQ1/8/5P1k/1K1n4/3p2b1/1pp1ppp1/3B4 w - - 0 1"); //composed to test promotion
    Engine::Position afterMove("2Qr4/1P1PPPQ1/8/5P1k/1K1n4/3p2b1/1pp1ppp1/3B4 b - - 0 1"); //after c8=Q

    Move move = 58 | (50<<6) | (11<< 12); //move c8=Q

    beforeMove.makeMove(move);

    expectPositionsEqual(beforeMove,afterMove);
}

TEST(Position, makeMovePromoteCapture){
    Engine::Position beforeMove("3r4/1PPPPPQ1/8/5P1k/1K1n4/3p2b1/1pp1ppp1/3B4 w - - 0 1"); //composed to test promotion
    Engine::Position afterMove("3Q4/1P1PPPQ1/8/5P1k/1K1n4/3p2b1/1pp1ppp1/3B4 b - - 0 1"); //after cxd8=Q

    Move move = 59 | (50<<6) | (15 << 12); //move cxd8=Q

    beforeMove.makeMove(move);

    expectPositionsEqual(beforeMove,afterMove);
}

// Unmake Move Tests

TEST(Position, unMakeMoveQuiet){
    Engine::Position beforeMove("r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4"); //Symmetrical Italian 3 ply deep
    Engine::Position afterMove("r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4"); //Symmetrical Italian 3 ply deep

    Move move = 12 | (3<<6); //move Qe2

    beforeMove.makeMove(move);
    beforeMove.unMakeMove(move);

    expectPositionsEqual(beforeMove,afterMove);
}

TEST(Position, unMakeMoveCapture){
    Engine::Position beforeMove("r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4"); //Symmetrical Italian 3 ply deep
    Engine::Position afterMove("r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4"); //Symmetrical Italian 3 ply deep

    Move move = 36 | (21<<6) |  (4 << 12) | (1 << 16); //move nxe5

    beforeMove.makeMove(move);
    beforeMove.unMakeMove(move);

    expectPositionsEqual(beforeMove,afterMove);
}

TEST(Position, unMakeMoveEP){
    Engine::Position beforeMove("rnbqkbnr/ppp2ppp/3p4/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6 0 3"); //d4d6d5e5
    Engine::Position afterMove("rnbqkbnr/ppp2ppp/3p4/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6 0 3"); //d4d6d5e5

    Move move = 44 | (35<<6) | (5 << 12)  | (1 << 16); //move dxe6

    beforeMove.makeMove(move);
    beforeMove.unMakeMove(move);

    expectPositionsEqual(beforeMove,afterMove);
}



TEST(Position, unMakeMoveKingsideCastle){
    Engine::Position beforeMove("r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4"); //Symmetrical Italian 3 ply deep
    Engine::Position afterMove("r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4"); //Symmetrical Italian 3 ply deep

    Move move = 6 | (4<<6) | (2 << 12); //move O-O

    beforeMove.makeMove(move);
    beforeMove.unMakeMove(move);

    expectPositionsEqual(beforeMove,afterMove);
}

TEST(Position, unMakeMoveQueensideCastle){
    Engine::Position beforeMove("rnb1kb1r/ppp2ppp/3q1n2/3pp3/3P1B2/2NQ4/PPP1PPPP/R3KBNR w KQkq - 2 5"); //d4d5Qd3Qd6Bf4e5Nc3Nf6
    Engine::Position afterMove("rnb1kb1r/ppp2ppp/3q1n2/3pp3/3P1B2/2NQ4/PPP1PPPP/R3KBNR w KQkq - 2 5"); //d4d5Qd3Qd6Bf4e5Nc3Nf6

    Move move = 2 | (4<<6) | (3 << 12); //move O-O-O

    beforeMove.makeMove(move);
    beforeMove.unMakeMove(move);

    expectPositionsEqual(beforeMove,afterMove);
}

//integration

TEST(Perft, startingPositionPerft2){
    Engine::MoveGenerator gen;
    Engine::Position pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"); //starting position

//    std::vector<Move> moves = gen.generateMoves(&pos);
//    for (const auto& move : moves){
//        Engine::printMove(move);
//    }
    auto start = std::chrono::high_resolution_clock::now();

    EXPECT_EQ(perftVerbose(&pos,&gen,1),400  );

    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

