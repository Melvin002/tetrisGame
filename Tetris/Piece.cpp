#include "Piece.h"

Piece::Piece(Pieces piece) {

	int pieceCode = 7;
	switch (piece) {
	case pieceO:
		shape[2][0] = pieceCode;
		shape[3][0] = pieceCode;
		shape[2][1] = pieceCode;
		shape[3][1] = pieceCode;
		break;
	case pieceI:
		shape[0][0] = pieceCode;
		shape[1][0] = pieceCode;
		shape[2][0] = pieceCode;
		shape[3][0] = pieceCode;
		break;
	case pieceS:
		shape[3][0] = pieceCode;
		shape[3][1] = pieceCode;
		shape[2][1] = pieceCode;
		shape[2][2] = pieceCode;
		break;
	case pieceZ:
		shape[2][0] = pieceCode;
		shape[2][1] = pieceCode;
		shape[3][1] = pieceCode;
		shape[3][2] = pieceCode;
		break;
	case pieceL:
		shape[3][0] = pieceCode;
		shape[3][1] = pieceCode;
		shape[2][0] = pieceCode;
		shape[1][0] = pieceCode;
		break;
	case pieceJ:
		shape[3][1] = pieceCode;
		shape[3][2] = pieceCode;
		shape[2][2] = pieceCode;
		shape[1][2] = pieceCode;
		break;
	case pieceT:
		shape[3][0] = pieceCode;
		shape[3][1] = pieceCode;
		shape[2][1] = pieceCode;
		shape[3][2] = pieceCode;
		break;
	}
}

void Piece::rotate(){
}

void Piece::move(Direction dir){
}
