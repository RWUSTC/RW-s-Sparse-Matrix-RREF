
# RW-s-Sparse-Matrix-RREF

Embedded Mathematica Interface for Sparse Matrices Reduction with SpaSM
This is a Mathematica Package to reduce sparse matrices to echelon form by C program with SpaSM package.
There are some codes from the examples of LibraryLink of Wolfram Mathematica and SpaSM.

## Description

It can make RREF much faster.

## Getting Started

### Dependencies

* SpaSM package installed on your pc.
* Better be used under ubuntu 20.04 and up.

### Installation

* When installing, put the SparseRREF.so to your /usr/local/lib (recommended,for the spasm library is located in the same place), and put folder SparseRREF to your /usr/local/Wolfram/Mathematica/12.X/Packages/.
*  If not work, add the following codes to your init.m (to get the path to this file, simply use        FileNameJoin[{$UserBaseDirectory, "Kernel", "init.m"}])
    
    
    $SparseRREFPath = "Your/Installation/Path/to/SparseRREF"
    If[Not[MemberQ[$Path,$SparseRREFPath]],$Path = Flatten[{$Path, $SparseRREFPath }]];
    $SparseRREFLibraryPath = "/usr/local/lib"
    If[Not[MemberQ[$LibraryPath,$SparseRREFLibraryPath]],$LibraryPath = Flatten[{$LibraryPath, $SparseRREFLibraryPath }]];
    
     (replace the path with yours in the codes)

### Executing program

* Simply use  "<<SparseRREF`" to enable this package
* Input form : currently only SparseArray
* Use "SRSparseRowReduce[SparseArray, Modulus->int_mod]" to perform rref.
* Warning: Integer Matrices Only! If using the form with quotients, please proceed it with the line in Mathematica:
   Cancel[SparseArray,Modulus->int_mod] 


## Authors

Hefeng Xu, Department of Modern Physics, USTC.
