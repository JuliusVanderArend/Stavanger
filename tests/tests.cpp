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

TEST(MoveGenerator, generateMoves){
    auto* gen = new Engine::MoveGenerator();
    auto* pos = new Engine::Position();

    std::vector<Move> generatedMoves = gen->generateMoves(pos);

    EXPECT_EQ(generatedMoves.size(),20);
}