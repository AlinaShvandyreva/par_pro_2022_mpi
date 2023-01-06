// Copyright 2022 Shvandyreva Alina
#include <gtest/gtest.h>
#include <vector>
#include "./min_diff.h"
#include <gtest-mpi-listener.hpp>

TEST(MIN_DIFF, TEST_N_128) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int n = 128;
    std::vector<int> vec;

    if (rank == 0) {
        vec = getRandomVector(n);
    }

    int indexParallel = minDiffParallel(vec, n);

    if (rank == 0) {
        int indexSequential = minDiffSequential(vec, n);
        ASSERT_EQ(indexParallel, indexSequential);
    }
}

TEST(MIN_DIFF, TEST_N_256) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int n = 256;
    std::vector<int> vec;

    if (rank == 0) {
        vec = getRandomVector(n);
    }

    int indexParallel = minDiffParallel(vec, n);

    if (rank == 0) {
        int indexSequential = minDiffSequential(vec, n);
        ASSERT_EQ(indexParallel, indexSequential);
    }
}

TEST(MIN_DIFF, TEST_N_512) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int n = 512;
    std::vector<int> vec;

    if (rank == 0) {
        vec = getRandomVector(n);
    }

    int indexParallel = minDiffParallel(vec, n);

    if (rank == 0) {
        int indexSequential = minDiffSequential(vec, n);
        ASSERT_EQ(indexParallel, indexSequential);
    }
}

TEST(MIN_DIFF, TEST_N_1024) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int n = 1024;
    std::vector<int> vec;

    if (rank == 0) {
        vec = getRandomVector(n);
    }

    int indexParallel = minDiffParallel(vec, n);

    if (rank == 0) {
        int indexSequential = minDiffSequential(vec, n);
        ASSERT_EQ(indexParallel, indexSequential);
    }
}

TEST(MIN_DIFF, TEST_N_2048) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int n = 2048;
    std::vector<int> vec;

    if (rank == 0) {
        vec = getRandomVector(n);
    }

    int indexParallel = minDiffParallel(vec, n);

    if (rank == 0) {
        int indexSequential = minDiffSequential(vec, n);
        ASSERT_EQ(indexParallel, indexSequential);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
