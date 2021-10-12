# RW-s-Sparse-Matrix-RREF
Embedded Mathematica Interface for Sparse Matrices Reduction with SpaSM
This is a Mathematica Package to reduce sparse matrices to echelon form by C program with SpaSM package.
There are some codes from the examples of LibraryLink of Wolfram Mathematica and SpaSM.
 ##Installation
  When installing, put the SparseRREF.so to your /usr/local/lib, and put folder SparseRREF to your /usr/local/Wolfram/Mathematica/12.X/Applications/.
  If not work, add the following codes to your /Autoload/init.m
    (*begin of SparseRREF*)
    $SparseRREFPath = "/usr/local/Wolfram/Mathematica/12.0/AddOns/Applications/SparseRREF"
    If[Not[MemberQ[$Path,$SparseRREFPath]],$Path = Flatten[{$Path, $SparseRREFPath }]];
    $SparseRREFLibraryPath = "/usr/local/lib"
    If[Not[MemberQ[$LibraryPath,$SparseRREFLibraryPath]],$LibraryPath = Flatten[{$LibraryPath, $SparseRREFLibraryPath }]];
    (*end of SparseRREF*)
#Using this package
  In Mathematica, type "<<SparseRREF`"
  Main
