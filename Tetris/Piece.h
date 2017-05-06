#pragma once
#include "Point.h"
#include <vector>

const Point DEFAULT_POSITION = Point(3, 1);

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
		shadow,
		empty,
		random
	};

	static std::vector<Pieces> randomPermutationOfPieces;
	Pieces pieceType;
	int pieceCode;
	void positionReset();

	Piece(Pieces pieceType);
private:
	void putPieceTypesAndShuffle();
};