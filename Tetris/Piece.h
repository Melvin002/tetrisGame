#pragma once
#include "Point.h"
#include <vector>

class Piece {
public:
	Point shape[4]; //all tetris shapes contains 4 squares
	Point position;
	enum Pieces {
		pieceO,
		pieceI,
		pieceS,
		pieceZ,
		pieceL,
		pieceJ,
		pieceT,
		shadow
	};

	static std::vector<Pieces> randomPermutationOfPieces;
	Pieces pieceType;
	int pieceCode;

	Piece();
private:
	void putPieceTypesAndShuffle();
};