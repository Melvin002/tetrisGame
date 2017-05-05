#include "Piece.h"
#include <algorithm>

std::vector<Piece::Pieces> Piece::randomPermutationOfPieces;


Piece::Piece() {
	if (randomPermutationOfPieces.empty()) {
		putPieceTypesAndShuffle();
	}
	pieceType = randomPermutationOfPieces.back();
	randomPermutationOfPieces.pop_back();
	
	position = Point(3, -1);
	switch (pieceType) {
	case pieceO:
		pieceType = pieceO;
		shape[0] = Point(0, 0);
		shape[1] = Point(0, 1);
		shape[2] = Point(1, 0);
		shape[3] = Point(1, 1);
		pieceCode = 100;
		break;
	case pieceI:
		pieceType = pieceI;
		shape[0] = Point(0, 1);
		shape[1] = Point(1, 1);
		shape[2] = Point(2, 1);
		shape[3] = Point(3, 1);
		pieceCode = 200;
		break;
	case pieceS:
		pieceType = pieceS;
		shape[0] = Point(2, 0);
		shape[1] = Point(1, 0);
		shape[2] = Point(1, 1);
		shape[3] = Point(0, 1);
		pieceCode = 300;
		break;
	case pieceZ:
		pieceType = pieceZ;
		shape[0] = Point(0, 0);
		shape[1] = Point(1, 0);
		shape[2] = Point(1, 1);
		shape[3] = Point(2, 1);
		pieceCode = 400;
		break;
	case pieceL:
		pieceType = pieceL;
		shape[0] = Point(0, 0);
		shape[1] = Point(1, 0);
		shape[2] = Point(2, 0);
		shape[3] = Point(0, 1);
		pieceCode = 500;
		break;
	case pieceJ:
		pieceType = pieceJ;
		shape[0] = Point(0, 0);
		shape[1] = Point(1, 0);
		shape[2] = Point(2, 0);
		shape[3] = Point(2, 1);
		pieceCode = 600;
		break;
	case pieceT:
		pieceType = pieceT;
		shape[0] = Point(1, 0);
		shape[1] = Point(0, 1);
		shape[2] = Point(1, 1);
		shape[3] = Point(2, 1);
		pieceCode = 700;
		break;
	}
}
int myrandom(int i) { 
	return std::rand() % i; 
}

void Piece::putPieceTypesAndShuffle(){
	for (int enumIterator = pieceO; enumIterator < shadow; enumIterator++){
		Pieces p = static_cast<Pieces>(enumIterator);
		randomPermutationOfPieces.push_back(p);
	}
	std::random_shuffle(randomPermutationOfPieces.begin(), randomPermutationOfPieces.end(), myrandom);
	
}


