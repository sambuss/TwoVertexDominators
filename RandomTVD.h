#pragma once
// *********************************************************
// RandomTVD.h -- Version 1.1. 
//    Version 1.0 - started November 24, 2024 
//    Version 1.1 - change to comment
// 
//     Author: Sam Buss (sbuss@ucsd.edu) with Vijay Ganesh and Albert Oliveras.
//     This code has no warranties of correctness or appropriateness.
//     May be used freely, however acknowledgement of use
//         is expected and appreciated.
//
//  This routine takes as input VertListA and VertListB as produced
//      by CalcDominators, and returns a randomly selected two-vertex
//      dominator (a,b). 
//  Optionally, the minimal pair is disallowed.
//  Randomization is on the function RandGenTVD(i), which must
//      be user supplied (function prototype is in this header file).
// *********************************************************

#ifndef RANDOM_TVD_H
#define RANDOM_TVD_H

#include <vector>
#include "TwoVertexDominators.h"

// ** RandGenTVD ** 
//    User supplied function, returns random integer in the range [0,n).
//    This allows the calling program to control random number generation.
int RandGenTVD(int n);      

// *********************
// ChooseRandomTVD
// Input is:
//     - the two Vert Lists from CalcDominators
//     - Optional argument avoidInit: whether the randomly chosen TVD can be the 
//                  first vertices from vertListA and vertListB.
// Returns:
//     - return code is TRUE if a randomly chosen TVD is returned.
//       return code is FALSE if there is no TVD available to return.
//     - return values vertIdxA and vertIdxB are the indices in vertListA and vertListB of a TVD.
//       I.e., vertListA[vertIdxA].vertNum and vertListB[vertIdxB].vertNum are the members of the two vertex dominator.
// *********************
bool ChooseRandomTVD(const std::vector<TwoVertexDominators::VertPairInfo>& vertListA, 
                     const std::vector<TwoVertexDominators::VertPairInfo>& vertListB, 
                     int& vertIdxA, int& vertIdxB, bool avoidInit = false);

#endif   // RANDOM_TVD_H
