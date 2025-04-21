//
// RandomTVD.cpp
//   Version 1.0, Started 11/24/2024
// .
//   Author: Sam Buss (sbuss@ucsd.edu) with Vijay Ganesh and Albert Oliveras.
//   This code has no warranties of correctness or appropriateness.
//   May be used freely, however acknowledgement of use 
//         is expected and appreciated.
// 

#include "RandomTVD.h"
#include "TwoVertexDominators.h"
#include <vector>


bool ChooseRandomTVD(
    const std::vector<TwoVertexDominators::VertPairInfo>& vertListA,
    const std::vector<TwoVertexDominators::VertPairInfo>& vertListB,
    int& vertIdxA, int& vertIdxB, bool avoidInit) {

    // Get the total number of TVDs
    int numTVDs = 0;
    for (const TwoVertexDominators::VertPairInfo& vpiA : vertListA) {
        numTVDs += vpiA.maxPairIdx - vpiA.minPairIdx + 1;
    }

    int avoidNum = (int)avoidInit;
    if (numTVDs <= avoidNum) {
        return false;
    }

    int randNum = RandGenTVD(numTVDs - avoidNum);
    assert(randNum >= 0 && randNum < numTVDs - avoidNum);
    int randomTVD = randNum + avoidNum;

    for (int i = 0; ; i++) {
        const TwoVertexDominators::VertPairInfo& vpiA = vertListA[i];
        int numPairs = vpiA.maxPairIdx - vpiA.minPairIdx + 1;
        if (numPairs > randomTVD) {
            vertIdxA = i;
            vertIdxB = vpiA.minPairIdx + randomTVD;
            return true;
        }
        randomTVD -= numPairs;
    }
}

