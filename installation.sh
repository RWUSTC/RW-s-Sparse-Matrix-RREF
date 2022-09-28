#!/bin/bash
cat >> $HOME/.Mathematica/Autoload/init.m << EOF
(*begin of SparseRREF*)
$SparseRREFPath = "/usr/local/Wolfram/Mathematica/12.0/AddOns/Applications/SparseRREF"
If[Not[MemberQ[$Path,$SparseRREFPath]],$Path = Flatten[{$Path, $SparseRREFPath }]];
$SparseRREFLibraryPath = "/usr/local/lib"
If[Not[MemberQ[$LibraryPath,$SparseRREFLibraryPath]],$LibraryPath = Flatten[{$LibraryPath, $SparseRREFLibraryPath }]];
(*end of SparseRREF*)
EOF

if [ $? -eq 0 ];
then
    echo "Successfully written in the init.m!"
else
    echo "Failed writting into init.m!"
fi

x=$(Mathematica --version)

sudo cp SparseRREF.so /usr/local/lib

if [ $? -eq 0 ];
then
    echo "Library successfully installed!"
else
    echo "Library installation failed!"
fi

sudo cp -r SparseRREF /usr/local/Wolfram/Mathematica/$x/AddOns/Applications

if [ $? -eq 0 ];
then
    echo "Package successfully installed!"
else
    echo "Package installation failed!"
fi

