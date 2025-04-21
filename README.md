# TwoVertexDominators
Software for two-vertex dominators detection and selection

Contents:

**TwoVertexDominators.cpp** and **TwoVertexDominators.h** - Code for finding the two vertex dominators (TVDs) between two vertices s and t in a topologically sorted, acyclic, directed graph. See TVDtester.cpp for example usage  For details on this algorithm, see:

Sam Buss, Elena Dubrova, Vijay Ganesh, Albert Oliveras and Maxim Teslenko, "A Linear Time Algorithm for Two-Vertex Dominators", preprint, 2024/2025.

Also see the earlier technical report: Maxim Teslenko and Elena Dubrova, "A Linear-Time Algorithm for Finding All Double-Vertex Dominators of a Given Vertex", arxiv:1503.04994v1, 2015.

**RandomTVD.cpp** and **RandomTVD.h** - Code for selecting a random TVD. 

**MostActiveTVD.h** - Code for finding a maximum weight TVD. (Can be easily adapted to find a minimum weight TVD. See the above-mention five author paper for algorithmic details.

**TVDtester.cpp**, **test-set-1.txt**, **test-set-2.txt**, **test-set-3.txt**. Test code and test data.  TVDtester.cpp shows usage exampes for the above routines.

Bug reports to: Sam Buss, sbuss@ucsd.edu or sambuss@gmail.com.
