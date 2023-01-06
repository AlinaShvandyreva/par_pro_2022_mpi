
// Copyright 2022 Shvandyreva Alina
#include <mpi.h>
#include <algorithm>
#include <cmath>
#include <random>
#include <vector>
#include "../../../modules/task_1/shvandyreva_a_min_diff/min_diff.h"

std::vector<int> getRandomVector(int n) {
    std::random_device device;
    std::mt19937 gen(device());
    std::vector<int> vec(n);
    for (int i = 0; i < n; ++i) {
        vec[i] = gen() % 10;
    }
    return vec;
}

int minDiffSequential(const std::vector<int> &vec, int n) {
    int index = 0;
    int diff = std::abs(vec[index] - vec[index + 1]);

    for (int i = 1; i < n - 1; ++i) {
        int loopDiff = std::abs(vec[i] - vec[i + 1]);
        if (loopDiff < diff) {
            diff = loopDiff;
            index = i;
        }
    }

    return index;
}

int minDiffParallel(const std::vector<int> &vec, int n) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int delta = n / size;

    std::vector<int> localVec(n);

    if (rank == 0) {
        localVec = vec;
    }

    MPI_Bcast(localVec.data(), n, MPI_INT, 0, MPI_COMM_WORLD);

    int localIndex = delta * rank;
    int localLimit = rank == size - 1 ? localIndex + delta - 1 : localIndex + delta;
    int localDiff = std::abs(localVec[localIndex] - localVec[localIndex + 1]);

    for (int i = localIndex + 1; i < localLimit; ++i) {
        int loopDiff = std::abs(localVec[i] - localVec[i + 1]);
        if (loopDiff < localDiff) {
            localDiff = loopDiff;
            localIndex = i;
        }
    }

    std::vector<int> ids(size);

    MPI_Gather(&localIndex, 1, MPI_INT, ids.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        int rootIndex = ids[0];
        int rootDiff = std::abs(vec[rootIndex] - vec[rootIndex + 1]);
        for (int i = 1; i < size; ++i) {
            int loopIndex = ids[i];
            int loopDiff = std::abs(vec[loopIndex] - vec[loopIndex + 1]);
            if (loopDiff < rootDiff) {
                rootDiff = loopDiff;
                rootIndex = loopIndex;
            }
        }

        return rootIndex;
    }

    return 0;
}
