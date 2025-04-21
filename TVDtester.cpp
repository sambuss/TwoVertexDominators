
// *******************************
// Tester coder for TwoVertexDominators.
// *******************************

#include "TwoVertexDominators.h"
#include "MostActiveTVD.h"
#include "RandomTVD.h"
#include <iostream>
#include <fstream>
#include <random>

//   ******  Next lines needed for MostActiveTVD ******
typedef int ActivityValue_t;
template <typename ActivityValue_t>
ActivityValue_t CombineActivities(ActivityValue_t aValue, ActivityValue_t bValue) {
    return aValue + bValue;
}

//   ****** Next lines needed for RandomTVD ******
int RandGenTVD(int n) {
    return rand() % n;
}



// See end of file for definition of:
template<typename ACTIVITY> ACTIVITY ActivityOf(int vertNum);

void SimpleTests();
void FileTests(const char* filename);
int CalcAndTestDominators(int N, const int preds[], const int predIdx[]);
bool CheckPair(int vertA, int vertB, int N, const int preds[], const int predIdx[]);
bool CheckAncestry(int startVert, int ancestor, int nonAncestor, int N, const int preds[], const int predIdx[]);

void PrecalcActivities(int numVerts);

bool AssertsActive();

int main() {

    assert(AssertsActive());

    SimpleTests();

    //FileTests("C:/Users/sbuss/Dropbox/C++Projects/TwoVertexDominators/Albert_Tester_Oct2023/test-set-1.txt");
    FileTests("Albert_Tester_Oct2023/test-set-1.txt");
    FileTests("Albert_Tester_Oct2023/test-set-2.txt");
    FileTests("Albert_Tester_Oct2023/test-set-3.txt");
    //FileTests("C:/Users/sambu/Dropbox/C++Projects/TwoVertexDominators/Albert_Tester_Oct2023/test-set-1.txt");
    //FileTests("C:/Users/sambu/Dropbox/C++Projects/TwoVertexDominators/Albert_Tester_Oct2023/test-set-2.txt");
    //FileTests("C:/Users/sambu/Dropbox/C++Projects/TwoVertexDominators/Albert_Tester_Oct2023/test-set-3.txt");
}

TwoVertexDominators myTVD;
void SimpleTests()
{
    int i;

    // Test "A": Simple path 0 - 2 - 4 - 6.  No second path.
    int N_A = 7;
    int preds_A[] =   { 2,3,4,5,6,6 };
    int predIdx_A[] = { 0,1,2,3,4,5,6 };
    i = CalcAndTestDominators(N_A, preds_A, predIdx_A);
    assert(i == -1);    // No second vertex-disjoint path

    // Test "B":  Like Test "A", but add edges 0 - 1 and 3 - 4 
    int N_B = 7;
    int preds_B[] =   { 2,1, 3, 4, 4,5, 6, 6 };
    int predIdx_B[] = { 0,   2, 3, 4,   6, 7, 8 };
    i = CalcAndTestDominators(N_B, preds_B, predIdx_B);
    assert(i == 1);    

    // Test "C":  Like Test "B", but with edge  4-5 instead of 3-5. Makes path reroute in Phase B. 
    int N_C = 7;
    int preds_C[] =   { 2,1, 3, 4, 4,   6,5,  6 };
    int predIdx_C[] = { 0,   2, 3, 4,   5,    7,  8 };
    i = CalcAndTestDominators(N_C, preds_C, predIdx_C);
    assert(i == -1);    // No second vertex-disjoint path

    // Test "D":  Like Test "C", but adding edge 2-5. Finds 2 paths, after re-routing. 
    int N_D = 7;
    int preds_D[] =   { 2,1, 3, 4,5, 4,   6,5,  6 };
    int predIdx_D[] = { 0,   2, 3,   5,   6,    8,  9 };
    i = CalcAndTestDominators(N_D, preds_D, predIdx_D);
    assert(i == 1);

    // Test "E":  Like Test "D", but adding edges 4-7,6-7,6-8,7-8. Finds 2 paths, after three times re-routing. 
    int N_E = 9;
    int preds_E[] =   { 2,1, 3, 4,5, 4,   6,5,7,  6,  7,8,  8 };
    int predIdx_E[] = { 0,   2, 3,   5,   6,      9,  10,   12, 13 };
    i = CalcAndTestDominators(N_E, preds_E, predIdx_E);
    assert(i == 1);

    // Test "F":  Like Test "E", but switches out-order of node 6. Finds 2 paths, after twice re-routing. 
    int N_F = 9;
    int preds_F[] =   { 2,1, 3, 4,5, 4,   6,5,7,  6,  8,7,  8 };
    int predIdx_F[] = { 0,   2, 3,   5,   6,      9,  10,   12, 13 };
    i = CalcAndTestDominators(N_F, preds_F, predIdx_F);
    assert(i == 1);

    // Test "G"
    int N_G = 12;
    int preds_G[] =   { 2,1, 3,4, 4,6, 5, 5, 10,9,6, 7, 8,9, 10, 11, 11 };
    int predIdx_G[] = { 0,   2,   4,   6, 7, 8,      11,12,  14, 15, 16, 17 };
    i = CalcAndTestDominators(N_G, preds_G, predIdx_G);
    assert(i == 1);

    // Test "H" - Albert's bug
    int N_H = 12;
    int preds_H[] = { 1,2,5, 3, 3, 6,8,4, 7, 7,  7,  9, 10, 11, 11 };
    int predIdx_H[] = { 0,     3, 4, 5,     8, 9, 10, 11, 12, 13, 14, 15 };
    i = CalcAndTestDominators(N_H, preds_H, predIdx_H);
    assert(i == 1);

    // Test "I" - Three simple, disjoint paths
    int N_I = 6;
    int preds_I[] =   { 1,2,3, 4, 5, 5, 5  };
    int predIdx_I[] = { 0,     3, 4, 5, 6, 7 };
    i = CalcAndTestDominators(N_I, preds_I, predIdx_I);
    assert(i == 0);

    // Test "J" - Trivial graph
    int N_J = 2;
    int preds_J[] = { 1 };
    int predIdx_J[] = { 0, 1 };
    i = CalcAndTestDominators(N_J, preds_J, predIdx_J);
    assert(i == 0);

    // Test "Bug_Nov06_2023" - From Albert's email
    int N_K = 30;
    int preds_K[] = { 1, 2, 26, 18, 10, 3, 3, 4, 7, 5, 6, 10, 13, 8, 9, 11, 11, 12, 15, 13, 14, 18, 21, 16, 17, 19, 19, 20, 23, 21, 22, 26, 29, 24, 25, 27, 27, 28, 29, -1 };
    int predIdx_K[] = { 0, 5, 6, 7, 9, 10, 12, 13, 14, 15, 16, 17, 19, 20, 22, 23, 24, 25, 26, 27, 29, 30, 32, 33, 34, 35, 36, 37, 38, 39, -1 };
    i = CalcAndTestDominators(N_K, preds_K, predIdx_K);
    assert(i == 1);

}

void FileTests(const char* filename) {
    std::ifstream testFile(filename);

    if (!testFile) {
        fprintf(stderr, "Open error: %s\n", filename);
    }

    std::string s;
    int example_num, N;

    while (testFile >> s >> example_num) {
        assert(s == "Example");
        //std::cout << "Example " << example_num << std::endl;

        testFile >> s >> N; 
        assert(s == "N");
        testFile >> s;
        assert(s == "predecessors:");
        std::vector<int> preds;
        int x;
        while (testFile >> x and x != -1) preds.push_back(x);
        testFile >> s;
        assert(s == "predIndex:");
        std::vector<int> predsIdx;
        while (testFile >> x and x != -1) predsIdx.push_back(x);

        assert(N == predsIdx.size());
        assert(predsIdx.back() == preds.size());
        assert(preds.back() == N - 1);

#if false
        int res = CalcAndTestDominators((int)predsIdx.size(), preds.data(), predsIdx.data());
        //std::cout << res << std::endl << std::endl;
#endif
#if true
        myTVD.CalcDominators((int)predsIdx.size(), preds.data(), predsIdx.data());
        int retA, retB;
        bool foundTVD = ChooseRandomTVD(myTVD.GetVertListA(), myTVD.GetVertListB(), retA, retB, false);
        assert(!foundTVD
            || ((retA < myTVD.GetVertListA().size() && retA >= 0) 
                && (retB < myTVD.GetVertListB().size() && retB >= 0)));
#endif
    }
    testFile.close();
    std::cout << example_num << " Examples Tested!" << std::endl;
}

bool AssertsActive() {
    std::cout << "Asserts are being checked" << std::endl;
    return true;
}

// Return value
//   CalcDominators return value if the tests find no problem
//   Some value <-100 if the tests find an error.
int CalcAndTestDominators(int N, const int preds[], const int predIdx[]) {
    int i = myTVD.CalcDominators(N, preds, predIdx);

    if (i == -2) {
        // Malformed directed graph!
        assert(false);
        return -2;
    }

    const std::vector<TwoVertexDominators::VertPairInfo>& listA = myTVD.GetVertListA();
    const std::vector<TwoVertexDominators::VertPairInfo>& listB = myTVD.GetVertListB();
    int lenA = (int)listA.size();
    int lenB = (int)listB.size();

    if (i <= 0 && !(listA.empty() && listB.empty())) {
        assert(false);
        return -100;
    }

    if (listA.empty() != listB.empty()) {
        assert(false);
        return -106;
    }

    if (listA.empty() && i > 0) {
        assert(false);
        return -107;
    }

#if 0 
    // Temporary testing for the transition from minPair and maxPair to minPairIdx and maxPairIdx
    for (int j = 0; j < lenA; j++) {
        int minK = listA[j].minPairIdx;
        assert(listA[j].minPair <= listB[minK].vertNum);
        assert(minK == 0 || listB[minK - 1].minPair < listA[j].minPair);
        int maxK = listA[j].maxPairIdx;
        assert(listA[j].maxPair >= listB[maxK].vertNum);
        assert(maxK == lenB - 1 || listB[maxK + 1].vertNum > listA[j].maxPair);
    }
#endif // XXX

    // Verify everything is increasing appropriately in both lists
    for (int AB = 0; AB < 2; AB++) {
        const std::vector<TwoVertexDominators::VertPairInfo>& curVPL = AB == 0 ? myTVD.GetVertListA() : myTVD.GetVertListB();
        int prevMinAncestor = 0;
        int prevLowerBdPair = 0;
        int prevUpperBdPair = 0;
        int prevVertNum = 0;
        for (int i = 0; i < (int)curVPL.size(); i++) {
            int curMinAncestor = curVPL[i].minAncestor;
            int curLowerBdPair = curVPL[i].minPairIdx;
            int curUpperBdPair = curVPL[i].maxPairIdx;
            int curVertNum = curVPL[i].vertNum;
            if (curMinAncestor < prevMinAncestor || curLowerBdPair < prevLowerBdPair || curUpperBdPair < prevUpperBdPair) {
                assert(false);
                return -101;
            }
            if (curLowerBdPair > curUpperBdPair) {
                assert(false);
                return -102;
            }
            if (curVertNum <= prevVertNum) {
                assert(false);
                return -103;
            }
            prevMinAncestor = curMinAncestor;
            prevLowerBdPair = curLowerBdPair;
            prevUpperBdPair = curUpperBdPair;
            prevVertNum = curVertNum;
        }
    }

    // Verify that the lower & upper bounds in lists A and B are consistent with each other
    //    (in fact, the information is redundant).
    for (int j = 0; j < lenA; j++) {
        for (int k = 0; k < lenB; k++) {
            if (listA[j].vertNum == listB[k].vertNum) {
                assert(false);
                return -110;
            }
            bool matchAtoB = (k <= listA[j].maxPairIdx && k >= listA[j].minPairIdx);
            bool matchBtoA = (j <= listB[k].maxPairIdx && j >= listB[k].minPairIdx);
            if (!(matchBtoA == matchAtoB)) {
                assert(false);
                return -105;
            }
        }
    }

    // Verify that every list A vert and every list B vert is in a two-vertex pair
    //   with at least one vertex in the other list.
    if (i > 0) {
        for (int AB = 0; AB < 2; AB++) {
            const std::vector<TwoVertexDominators::VertPairInfo>& curVPL = AB == 0 ? myTVD.GetVertListA() : myTVD.GetVertListB();
            const std::vector<TwoVertexDominators::VertPairInfo>& otherVPL = AB != 0 ? myTVD.GetVertListA() : myTVD.GetVertListB();
            int prevMax = 0;
            for (int j = 0; j < (int)curVPL.size(); j++) {
                if (!(curVPL[j].minPairIdx <= prevMax + 1)) {
                    assert(false);
                    return -109;
                }
                prevMax = curVPL[j].maxPairIdx;
            }
            if (!(curVPL.back().maxPairIdx == otherVPL.size() - 1)) {
                assert(false);
                return -109;
            }
        }
    }

    // Check correctness of minAncestor values in ListA and ListB
    for (int AB = 0; AB < 2; AB++) {
        const std::vector<TwoVertexDominators::VertPairInfo>& list = (AB == 0) ? listA : listB;
        int nonAncestor = 0;
        for (int j = 0; j < (int)list.size(); j++) {
            int ancestor = list[j].minAncestor;
            if ( (j == list.size() - 1 || ancestor < list[j + 1].minAncestor) && ancestor < N - 1 ) {
                // Only check ancestry with the minAncestor changes
                if (!CheckAncestry(list[j].vertNum, ancestor, nonAncestor, N, preds, predIdx)) {
                    assert(false);
                    return -110;
                }
                nonAncestor = ancestor;
            }
        }
    }

    if (i >= 0) {
        // If 2-connected or 3-connected (or more)
        // Check correctness of the vertex dominators
        // Loop over all pairs ii,jj as appropriate to check whether they form a two-vertex dominator (TVD)
        auto iterAii = listA.begin();
        auto iterBii = listB.begin();
        for (int ii = 1; ii < N - 1; ii++) {
            assert(iterAii == listA.end() || ii <= iterAii->vertNum);
            bool ii_inListA = iterAii != listA.end() && ii == iterAii->vertNum;
            assert(iterBii == listB.end() || ii <= iterBii->vertNum);
            bool ii_inListB = iterBii != listB.end() && ii == iterBii->vertNum;
            int iiIdxInA = (int)(iterAii - listA.begin());
            int iiIdxInB = (int)(iterBii - listB.begin());
            auto iterAjj = listA.begin();
            auto iterBjj = listB.begin();
            for (int jj = 1; jj <= ii; jj++) {
                assert(iterAjj == listA.end() || jj <= iterAjj->vertNum);
                bool jj_inListA = iterAjj != listA.end() && jj == iterAjj->vertNum;
                assert(iterBjj == listB.end() || jj <= iterBjj->vertNum);
                bool jj_inListB = iterBjj != listB.end() && jj == iterBjj->vertNum;
                bool expectedResult;
                // In i==0 (3-connected) case, listA and listB are empty, so expectedResult is false.
//                expectedResult = i > 0 && ii != jj &&
//                    ((ii_inListA && jj_inListB && ii >= iterBjj->minPair && ii <= iterBjj->maxPair)
//                        || (ii_inListB && jj_inListA && ii >= iterAjj->minPair && ii <= iterAjj->maxPair));
                expectedResult = i > 0 && ii != jj &&
                    ((ii_inListA && jj_inListB && iiIdxInA >= iterBjj->minPairIdx && iiIdxInA <= iterBjj->maxPairIdx)
                        || (ii_inListB && jj_inListA && iiIdxInB >= iterAjj->minPairIdx && iiIdxInB <= iterAjj->maxPairIdx));
                if (expectedResult != CheckPair(ii, jj, N, preds, predIdx)) {
                    assert(false);
                    return -150;
                }
                if (jj_inListA == true) {
                    iterAjj++;
                }
                if (jj_inListB == true) {
                    iterBjj++;
                }
            }
            if (ii_inListA == true) {
                iterAii++;
            }
            if (ii_inListB == true) {
                iterBii++;
            }
        }
    }
    else {
        // The non-2-connected case
        for (int ii = 1; ii <= myTVD.SingleVertDominator; ii++) {
            if ((ii == myTVD.SingleVertDominator) != CheckPair(ii, ii, N, preds, predIdx)) {
                assert(false);
                return - 160;
            }
        }
    }

    return i;
    // Verify MaxActivity
    if (i > 0) {
        // (a) Two computations give the same result.
        PrecalcActivities(N);
        const std::vector<TwoVertexDominators::VertPairInfo>& vertListA = myTVD.GetVertListA();
        const std::vector<TwoVertexDominators::VertPairInfo>& vertListB = myTVD.GetVertListB();
        int bestA_a, bestA_b, bestB_a, bestB_b;
        ActivityValue_t bestA, bestB;
        bestA = FindMaxActivityTVD<ActivityValue_t>(vertListA, vertListB, bestA_a, bestA_b);
        bestB = FindMaxActivityTVD<ActivityValue_t>(vertListB, vertListA, bestB_a, bestB_b);
        assert(ActivityOf<ActivityValue_t>(bestA_a) + ActivityOf<ActivityValue_t>(bestA_b) == bestA);
        assert(ActivityOf<ActivityValue_t>(bestB_a) + ActivityOf<ActivityValue_t>(bestB_b) == bestB);
        assert(bestA == bestB);

        // (b) Check all TVDs to make sure these are the max activity possibilities.
        for (int ia = 0; ia < vertListA.size(); ia++) {
            int vertNumA = vertListA[ia].vertNum;
            ActivityValue_t activeA = ActivityOf<ActivityValue_t>(vertNumA);
            for (int ib = 0; ib < vertListB.size(); ib++) {
                if (ia >= vertListB[ib].minPairIdx && ia <= vertListB[ib].maxPairIdx) {
                    ActivityValue_t thisActive = activeA + ActivityOf<ActivityValue_t>(vertListB[ib].vertNum);
                    assert(bestA >= thisActive);
                }
            }
        }
    }

    // The tests all succeeded!
    return i;
}

// CheckPair returns true if vertA and vertB form a two vertex dominator.
// Otherwise returns false
// Note vertA might equal vertB. 
//      (It should return false in this case -- only after verification however.)
bool CheckPair(int vertA, int vertB, int N, const int preds[], const int predIdx[]) {
    assert(vertA > 0 && vertA < N - 1 && vertB > 0 && vertB < N - 1);
    // Do a depth first search starting at the sink (vertex 0).
    std::vector<bool> visited(N, false);
    std::vector<int> stack;
    stack.push_back(0);     // Start at the sink
    while (!stack.empty()) {
        int exploreVert = stack.back();       // Next vertex for DFS
        stack.pop_back();
        if ( exploreVert != vertA && exploreVert != vertB) {
            // Loop over the predecessors of exploreVert
            int index = predIdx[exploreVert];
            for (int index = predIdx[exploreVert]; index < predIdx[exploreVert + 1]; index++) {
                int predVert = preds[index];             // Next predecessor of exploreVert
                if (predVert == N - 1) {
                    return false;           // Bypassed both vertA and vertB - not a 2-vertex dominator
                }
                if (!visited[predVert]) {
                    visited[predVert] = true;
                    stack.push_back(predVert);
                }
            }
        }
    }
    return true;  // Never reached N-1 -- they do form a 2-vertex dominator. (assuming vertA, vertB distinct)
}

// CheckAncestry returns true if and only ancestor is a proper ancestor of startVert
//    and nonAncestor is not.
// As implemented startVert is not considered an ancestor of itself.
bool CheckAncestry(int startVert, int ancestor, int nonAncestor, int N, const int preds[], const int predIdx[]) {
    // Do a depth first search starting at startVert
    std::vector<bool> visited(N, false);
    std::vector<int> stack;
    stack.push_back(startVert);
    while (!stack.empty()) {
        int exploreVert = stack.back();       // Next vertex for DFS
        stack.pop_back();
        // Loop over the predecessors of exploreVert
        if (exploreVert < N - 1) {
            for (int index = predIdx[exploreVert]; index < predIdx[exploreVert + 1]; index++) {
                int predVert = preds[index];             // Next predecessor of exploreVert
                if (!visited[predVert]) {
                    visited[predVert] = true;
                    stack.push_back(predVert);
                }
            }
        }
    }
    return visited[ancestor] && !visited[nonAncestor];
}

// ****************************************
// Testing routines for the MostActiveTVD routines.
// ****************************************
 
std::vector<ActivityValue_t> theActivities;

void PrecalcActivities(int numVerts) {
    theActivities.clear();
    for (int i = 0; i < numVerts; i++) {
        theActivities.push_back(rand() % 1000000);
    }
}

template<typename ACTIVITY>
ACTIVITY ActivityOf(int vertNum) {
    return theActivities[vertNum];
}
