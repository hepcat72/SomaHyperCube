CC=gcc

#puzzle: SolutionList.o PieceList.o Solution.o RotationList.o TranslationList.o Piece.o SolutionState.o DimensionalUnitTree.o puzzle.o
#	CC -o puzzle SolutionList.o PieceList.o Solution.o RotationList.o TranslationList.o Piece.o SolutionState.o DimensionalUnitTree.o puzzle.o
puzzle: SolutionList.o PieceList.o Solution.o RotationList.o TranslationList.o Piece.o SolutionState.o DimensionalUnitTree.o
	CC -std=c++11 -c puzzle.cpp
	clang++ -stdlib=libc++ -o puzzle SolutionList.o PieceList.o Solution.o RotationList.o TranslationList.o Piece.o SolutionState.o DimensionalUnitTree.o puzzle.o

SolutionList.o: SolutionList.cpp
	CC -std=c++11 -c SolutionList.cpp

PieceList.o: PieceList.cpp
	CC -std=c++11 -c PieceList.cpp

Solution.o: Solution.cpp
	CC -std=c++11 -c Solution.cpp
	
RotationList.o: RotationList.cpp
	CC -std=c++11 -c RotationList.cpp

TranslationList.o: TranslationList.cpp
	CC -std=c++11 -c TranslationList.cpp

Piece.o: Piece.cpp
	CC -std=c++11 -c Piece.cpp

SolutionState.o: SolutionState.cpp
	CC -std=c++11 -c SolutionState.cpp

DimensionalUnitTree.o: DimensionalUnitTree.cpp
	CC -std=c++11 -c DimensionalUnitTree.cpp

puzzle.o: puzzle.cpp
	CC -std=c++11 -c puzzle.cpp

clean:
	rm -rf ii_files puzzle SolutionList.o PieceList.o Solution.o RotationList.o TranslationList.o Piece.o DimensionalUnitTree.o SolutionState.o puzzle.o

all: puzzle
