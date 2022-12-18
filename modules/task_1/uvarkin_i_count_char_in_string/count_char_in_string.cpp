// Copyright 2022 Uvarkin Ilya
#include "count_char_in_string.h"
#include "mpi.h"

int getCountCharInString(const char* str, char ch){
    int count = 0;

    for (int i = 0; str[i] != '\0'; ++i) {
        if(str[i] == ch){
            count++;
        }
    }

    return count;
}

int getLenStringForRankProcess(int len, int rank, int procNum){
    int tail = len % procNum;
    int baseLenSubStr = len/procNum;

    if(rank >= procNum - tail){
        return baseLenSubStr+1;
    }

    return baseLenSubStr;
}

int getFirstIndexString(int len, int rank, int procNum){
    int tail = len % procNum;
    int baseLenSubStr = len/procNum;

    int tails = rank - (procNum - tail);
    if(tails > 0){
        return rank*baseLenSubStr+tails;
    }

    return rank*baseLenSubStr;
}

int getCountCharInStringParallel(const std::string& str, char letter){
    int count = 0;
    int procNum;
    int procRank;

    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    if (procRank == 0){
        for (int i = 1; i < procNum; ++i) {
            int first = getFirstIndexString(str.length(), i, procNum);
            int len = getLenStringForRankProcess(str.length(), i, procNum);

            std::string substr = str.substr(first,len);
            MPI_Send(substr.c_str(), substr.length(), MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }

        std::string substr = str.substr(0, getLenStringForRankProcess(str.length(), procRank, procNum));
        //printf("proc:%d. STR:%s\n", procRank, substr.c_str());
        count += getCountCharInString(substr.c_str(), letter);
        //printf("proc:%d. count:%d\n", procRank, count);
    } else{
        int len = getLenStringForRankProcess(str.length(), procRank, procNum);
        char *chars = new char[len+1];

        MPI_Status status;
        MPI_Recv(chars, len, MPI_CHAR, MPI_ANY_SOURCE , MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        chars[len] = '\0';
        //printf("proc:%d. STR:%s\n", procRank, chars);

        count += getCountCharInString(chars, letter);
        //printf("proc:%d. count:%d\n", procRank, count);
    }

    int res;
    MPI_Reduce(&count, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return res;
}
