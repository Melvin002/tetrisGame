#include "Piece.h"
#include <algorithm>

std::vector<Piece::Pieces> Piece::randomPermutationOfPieces;

Piece::Piece() {
	if (randomPermutationOfPieces.empty()) {
		putPieceTypesAndShuffle();
	}
	pieceType = randomPermutationOfPieces.back();
	randomPermutationOfPieces.pop_back();
	
	position = Point(3, 0);
	pieceCode = 7;
	switch (pieceType) {
	case pieceO:
		pieceType = pieceO;
		shape[0] = Point(0, 0);
		shape[1] = Point(0, 1);
		shape[2] = Point(1, 0);
		shape[3] = Point(1, 1);
		break;
	case pieceI:
		pieceType = pieceI;
		shape[0] = Point(0, 1);
		shape[1] = Point(1, 1);
		shape[2] = Point(2, 1);
		shape[3] = Point(3, 1);
		break;
	case pieceS:
		pieceType = pieceS;
		shape[0] = Point(2, 0);
		shape[1] = Point(1, 0);
		shape[2] = Point(1, 1);
		shape[3] = Point(0, 1);
		break;
	case pieceZ:
		pieceType = pieceZ;
		shape[0] = Point(0, 0);
		shape[1] = Point(1, 0);
		shape[2] = Point(1, 1);
		shape[3] = Point(2, 1);
		break;
	case pieceL:
		pieceType = pieceL;
		shape[0] = Point(0, 0);
		shape[1] = Point(1, 0);
		shape[2] = Point(2, 0);
		shape[3] = Point(0, 1);
		break;
	case pieceJ:
		pieceType = pieceJ;
		shape[0] = Point(0, 0);
		shape[1] = Point(1, 0);
		shape[2] = Point(2, 0);
		shape[3] = Point(2, 1);
		break;
	case pieceT:
		pieceType = pieceT;
		shape[0] = Point(1, 0);
		shape[1] = Point(0, 1);
		shape[2] = Point(1, 1);
		shape[3] = Point(2, 1);
		break;
	}
}

void Piece::putPieceTypesAndShuffle(){
	for (int enumIterator = pieceO; enumIterator <= pieceT; enumIterator++){
		Pieces p = static_cast<Pieces>(enumIterator);
		randomPermutationOfPieces.push_back(p);
	}
	std::random_shuffle(randomPermutationOfPieces.begin(), randomPermutationOfPieces.end());
	
}

