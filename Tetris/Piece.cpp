#include "Piece.h"

Piece::Piece(Pieces piece) {
	x = 0;
	y = 0;

	for (int i = 0; i < 4; i++) //4 width and heigth of piece array
		for (int j = 0; j < 4; j++)
			shape[i][j] = 0;
	int pieceCode = 7;
	switch (piece) {
	case pieceO:
		pieceType = pieceO;
		shape[2][0] = pieceCode;
		shape[3][0] = pieceCode;
		shape[2][1] = pieceCode;
		shape[3][1] = pieceCode;
		break;
	case pieceI:
		pieceType = pieceI;
		shape[1][0] = pieceCode;
		shape[1][1] = pieceCode;
		shape[1][2] = pieceCode;
		shape[1][3] = pieceCode;
		break;
	case pieceS:
		pieceType = pieceS;
		shape[3][0] = pieceCode;
		shape[3][1] = pieceCode;
		shape[2][1] = pieceCode;
		shape[2][2] = pieceCode;
		break;
	case pieceZ:
		pieceType = pieceZ;
		shape[2][0] = pieceCode;
		shape[2][1] = pieceCode;
		shape[3][1] = pieceCode;
		shape[3][2] = pieceCode;
		break;
	case pieceL:
		pieceType = pieceL;
		shape[3][0] = pieceCode;
		shape[3][1] = pieceCode;
		shape[2][0] = pieceCode;
		shape[1][0] = pieceCode;
		break;
	case pieceJ:
		pieceType = pieceJ;
		shape[3][1] = pieceCode;
		shape[3][2] = pieceCode;
		shape[2][2] = pieceCode;
		shape[1][2] = pieceCode;
		break;
	case pieceT:
		pieceType = pieceT;
		shape[3][0] = pieceCode;
		shape[3][1] = pieceCode;
		shape[2][1] = pieceCode;
		shape[3][2] = pieceCode;
		break;
	}
}

void Piece::rotate(){
}

