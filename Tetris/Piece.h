#pragma once
class Piece {
	/* All tetris shapes fits in 4x4 array */
public:
	int shape[4][4];
	enum Pieces {
		pieceO = 1,
		pieceI = 2,
		pieceS = 3,
		pieceZ = 4,
		pieceL = 5,
		pieceJ = 6,
		pieceT = 7,
	};

	int x; 
	int y;
	Pieces pieceType;

	Piece(Pieces piece);
private:
	void rotate();
};