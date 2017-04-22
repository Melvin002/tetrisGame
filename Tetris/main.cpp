#include <SFML/Graphics.hpp>
#include <string>
#include <queue>
#include "Piece.h"

const int FPS_NUMBER = 60;
const int MULTIPLER = 300;
const int WIDTH = 1 * MULTIPLER;
const int HEIGTH = 2 * MULTIPLER;
bool activePiece = false; // tez zmienic bo gowno, mowi czy losowac klocek czy nie

Piece piece(Piece::pieceI);//piece is global for now


enum Directions {
	Left, Right, DropDown, SoftDown
};

void processInput(sf::RenderWindow &window);
void update(int board[HEIGTH][WIDTH], Piece piece);
void render(sf::RenderWindow &window, int board[HEIGTH][WIDTH]);
void move(Directions direction);
void rotate(Directions direction);

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
	int board[HEIGTH][WIDTH] = { 0 };

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
	//keyboard input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
		move(Left);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
		move(Right);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		move(SoftDown);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		move(DropDown);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
		rotate(Right);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
		rotate(Left);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
		//hold
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		//pause in future
	}
}
int counter = 0;
void update(int board[HEIGTH][WIDTH], Piece piece) {
	//piece fall speed
	counter++;
	if (counter > 60) {//60 speed fall limiter
		move(SoftDown);
		counter = 0;
	}
		
	/*if (activePiece == false) {
		Pieces piece = static_cast<Pieces>(rand() % 7); //7 number of enums
		activePiece = true;
	}*/
	/*for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (piece.shape[i][j] != 0) {
				board[piece.x+j][piece.y+i] = piece.shape[i][j];
			}
		}
	}*/

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
	//render moving piece
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (piece.shape[j][i] != 0) {
				rectangle.setPosition((piece.x + i) * 30, (piece.y + j) * 30);
				window.draw(rectangle);
			}
		}
	}
	

	window.display();
}
void move(Directions direction){
	switch (direction) {
	case Left:
		piece.x--;
		break;
	case Right:
		piece.x++;
		break;
	case SoftDown:
		piece.y++;
		break;

	}
	
}
void rotate(Directions direction) {
	int tempArr[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tempArr[i][j] = piece.shape[i][j];
		}
	}
	switch (direction) {
	case Left:
		if (piece.pieceType != piece.pieceI) {
			for (int i = 1; i < 4; i++) {
				for (int j = 0; j < 3; j++) {
					piece.shape[i][j] = tempArr[3 - j][i - 1];
				}
			}
		}
		else {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					piece.shape[i][j] = tempArr[3 - j][i];
				}
			}
		}
		break;
	case Right:
		if (piece.pieceType != piece.pieceI) {
			for (int i = 1; i < 4; i++) {
				for (int j = 0; j < 3; j++) {
					piece.shape[i][j] = tempArr[j + 1][3 - i];
				}
			}
		}
		else {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					piece.shape[i][j] = tempArr[j][3 - i];
				}
			}
		}
		break;
	}
}
void clear() {

}