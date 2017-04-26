#pragma once
#include "Point.h"
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
	};

	Pieces pieceType;
	int pieceCode;

	Piece(Pieces piece);
private:
	void rotate();
};