
# RW-s-Sparse-Matrix-RREF

Embedded Mathematica Interface for Sparse Matrices Reduction with SpaSM
This is a Mathematica Package to reduce sparse matrices to echelon form by C program with SpaSM package.
There are some codes from the examples of LibraryLink of Wolfram Mathematica and SpaSM.

## Description

It can make RREF much faster.

## Getting Started

### Dependencies

* Currently included SpaSM package as libspasm.so in this module, no need to install. SpaSM on github: https://github.com/cbouilla/spasm
* Better be used under ubuntu 20.04 and up.
* Currently tested under Mathematica 12.0 and 13.0.

### Installation

* Open Mathematica, run "Get["$PackagePath/SparseRREF.m"];

### Executing program

* Simply use  "<<SparseRREF`" to enable this package
* Input form : currently only SparseArray
* Use "SRSparseRowReduce[SparseArray, Modulus->int_mod]" to perform rref.
* Warning: Integer Matrices Only! If using the form with quotients, please proceed it with the line in Mathematica:
   Cancel[SparseArray,Modulus->int_mod] 


## Author

Hefeng Xu, Department of Modern Physics, USTC.
