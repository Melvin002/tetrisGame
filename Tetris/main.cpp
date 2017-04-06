#include <SFML/Graphics.hpp>
#include <string>
#include <queue>
#include "Piece.h"

const int FPS_NUMBER = 60;
const int MULTIPLER = 300;
const int WIDTH = 1 * MULTIPLER;
const int HEIGTH = 2 * MULTIPLER;
bool activePiece = false; // tez zmienic bo gowno, mowi czy losowac klocek czy nie



int O[4][4] = {
	{0,0,0,0},
	{0,0,0,0},
	{0,7,0,0},
	{7,7,7,0}
};

void processInput(sf::RenderWindow &window);
void update(int board[HEIGTH][WIDTH], Piece piece);
void render(sf::RenderWindow &window, int board[HEIGTH][WIDTH]);

int main(){
	/*
	piece codes:
	o - 1,	i - 2,	s - 3,	z - 4
	l - 5,	j - 6,	t - 7

	XYYY
	X - type of piece code
	YYY - number of element

	0 - empty
	*/
	int board[HEIGTH][WIDTH];
	Piece piece(Piece::pieceS);

	sf::RenderWindow window;
	window.create(sf::VideoMode(WIDTH, HEIGTH), "Tetris!");
	window.setFramerateLimit(FPS_NUMBER);

	sf::Clock clock;
	sf::Time programTime;
	while (window.isOpen()) {
		
		processInput(window);
		update(board, piece);
		render(window, board);

	}

}
void processInput(sf::RenderWindow &window) {
	//sfml window event capture
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
	}
}
void update(int board[HEIGTH][WIDTH], Piece piece) {
	/*if (activePiece == false) {
		Pieces piece = static_cast<Pieces>(rand() % 7); //7 number of enums
		activePiece = true;
	}*/
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (piece.shape[i][j] != 0) {
				board[j][i] = piece.shape[i][j];
			}
		}
	}

}
void render(sf::RenderWindow &window, int board[HEIGTH][WIDTH]) {
	sf::RectangleShape rectangle(sf::Vector2f(30, 30));
	rectangle.setFillColor(sf::Color(119, 221, 119, 255));

	window.clear(sf::Color::Black);
	//przelatuje po planszy i rysuje kwadracik tam gdzie cos ma byc - chujowe trzeba bedzie zmienic
	for (int i = 0; i < HEIGTH; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (board[i][j] != 0) {
				rectangle.setPosition(i * 30, j * 30);
				window.draw(rectangle);
			}
		}
	}
	

	window.display();
}

/*int generatePieceCode(Pieces pieceType) {
	int returnPieceCode;
	switch(pieceType){
	case pieceO:
		returnPieceCode = pieceO;
		break;
	case pieceI:
		returnPieceCode = pieceI;
		break;
	case pieceS:
		returnPieceCode = pieceS;
		break;
	case pieceZ:
		returnPieceCode = pieceZ;
		break;
	case pieceL:
		returnPieceCode = pieceL;
		break;
	case pieceJ:
		returnPieceCode = pieceJ;
		break;
	case pieceT:
		returnPieceCode = pieceT;
		break;

	return returnPieceCode;
	}
}*/
