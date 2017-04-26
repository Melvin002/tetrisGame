#include <SFML/Graphics.hpp>
#include <string>
#include <queue>
#include "Piece.h"
#include <time.h> 
#include <stdlib.h> 

const int FPS_NUMBER = 60;
const int MULTIPLER = 300;
const int WIDTH = 1 * MULTIPLER;
const int HEIGTH = 2 * MULTIPLER;
const int BOARD_WIDTH = 10;
const int BOARD_HEIGTH = 20;
bool activePiece = false; // tez zmienic bo gowno, mowi czy losowac klocek czy nie
Piece piece = Piece::pieceO; // just initializing no matter with what



enum Directions {
	Left, Right, DropDown, SoftDown
};

void processInput(sf::RenderWindow &window);
void update(int board[BOARD_HEIGTH][BOARD_WIDTH]);
void render(sf::RenderWindow &window, int board[BOARD_HEIGTH][BOARD_WIDTH]);
void move(Directions direction);
void rotate(Directions direction);
void checkIfLanded(int board[BOARD_HEIGTH][BOARD_WIDTH]);

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
	srand(time(NULL));
	int board[BOARD_HEIGTH][BOARD_WIDTH] = { 0 };

	sf::RenderWindow window;
	window.create(sf::VideoMode(WIDTH, HEIGTH), "Tetris!");
	window.setFramerateLimit(FPS_NUMBER);
	//window.setKeyRepeatEnabled(false);

	sf::Clock clock;
	sf::Time programTime;

	piece = Piece(static_cast<Piece::Pieces>(rand() % 7));//piece is global for now

	while (window.isOpen()) {
		
		processInput(window);
		update(board);
		render(window, board);

	}

}
void processInput(sf::RenderWindow &window) {
	//sfml window event capture
	sf::Event event;
	while (window.pollEvent(event)){
		if (event.type == sf::Event::Closed)
			window.close();
		//keyboard input
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) {
			move(Left);
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right) {
			move(Right);
		}
		if (event.type == sf::Event::KeyPressed &&(event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::X)) {
			rotate(Right);
			window.setKeyRepeatEnabled(false);
		}
		if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::LControl || event.key.code == sf::Keyboard::Z)) {
			rotate(Left);
			window.setKeyRepeatEnabled(false);
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
			move(SoftDown);
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
			move(DropDown);
			piece = Piece(static_cast<Piece::Pieces>(rand() % 7));
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
			//pause in future
		}
		if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::C)) {
			//hold
		}
		if (event.type ==sf::Event::KeyReleased) {
			window.setKeyRepeatEnabled(true);
		}

	}

}
int counter = 0;
void update(int board[BOARD_HEIGTH][BOARD_WIDTH]) {
	//piece fall speed
	counter++;
	if (counter > 60) {//60 speed fall limiter
		move(SoftDown);
		counter = 0;
	}

	//
	/*int x[4];
	int y[4];
	int iterator = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (piece.shape[i][j] != 0) {
				x[iterator] = j + piece.x;
				y[iterator] = i + piece.y;
				iterator++;
			}
		}
	}
	piece.mostBottomY = y[0];
	for (int i = 1; i < 4; i++) {
		if (y[i] > piece.mostBottomY)
			piece.mostBottomY = y[i];
	}
	piece.mostLeftX = x[0];
	piece.mostRightX = x[0];

	for (int i = 1; i < 4; i++) {
		if (x[i] > piece.mostRightX)
			piece.mostRightX = x[i];
		if (x[i] < piece.mostLeftX)
			piece.mostLeftX = x[i];
	}*/
	
	checkIfLanded(board);
	/*for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (piece.shape[i][j] != 0) {
				board[piece.x+j][piece.y+i] = piece.shape[i][j];
			}
		}
	}*/

}
void render(sf::RenderWindow &window, int board[BOARD_HEIGTH][BOARD_WIDTH]) {
	sf::RectangleShape rectangle(sf::Vector2f(30, 30));
	rectangle.setFillColor(sf::Color(119, 221, 119, 255));

	window.clear(sf::Color::Black);
	//przelatuje po planszy i rysuje kwadracik tam gdzie cos ma byc - chujowe trzeba bedzie zmienic
	for (int i = 0; i < BOARD_HEIGTH; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			if (board[i][j] != 0) {
				rectangle.setPosition(j * 30, i * 30);
				window.draw(rectangle);
			}
		}
	}
	//render moving piece
	for (int i = 0; i < 4; i++){
		rectangle.setPosition((piece.shape[i].x + piece.position.x) * 30, (piece.shape[i].y + piece.position.y) * 30);
		window.draw(rectangle);
	}
	

	window.display();
}
void move(Directions direction){

	switch (direction) {
	case Left:
			piece.position.x--;
		break;
	case Right:
			piece.position.x++;
		break;
	case SoftDown:
		piece.position.y++;
		break;

	}
	
}
void rotate(Directions direction) {
	switch (direction) {
	case Right:
		if (piece.pieceType != piece.pieceI && piece.pieceType != piece.pieceO) {
			for (int i = 0; i < 4; i++) {
				int tempY = piece.shape[i].y;
				piece.shape[i].y = piece.shape[i].x;
				piece.shape[i].x = 2 - tempY;
			}
		}
		else if(piece.pieceType != piece.pieceO){
			for (int i = 0; i < 4; i++) {
				int tempY = piece.shape[i].y;
				piece.shape[i].y = piece.shape[i].x;
				piece.shape[i].x = 3 - tempY;
			}
		}
		break;
	case Left:
		if (piece.pieceType != piece.pieceI) {
			for (int i = 0; i < 4; i++) {
				int tempX = piece.shape[i].x;
				piece.shape[i].x = piece.shape[i].y;
				piece.shape[i].y = 2 - tempX;
			}
		}
		else if (piece.pieceType != piece.pieceO) {
			for (int i = 0; i < 4; i++) {
				int tempX = piece.shape[i].x;
				piece.shape[i].x = piece.shape[i].y;
				piece.shape[i].y = 3 - tempX;
			}
		}
		break;
	}
}
void checkIfLanded(int board[BOARD_HEIGTH][BOARD_WIDTH]) {
	bool pieceLanded = false;
	for (int i = 0; i < 4; i++) {
		if (board[piece.shape[i].y + piece.position.y + 1][piece.shape[i].x + piece.position.x] != 0)
			pieceLanded = true;
	}
	if (pieceLanded == true) {
		//write piece to board
		for (int i = 0; i < 4; i++) {
			board[piece.shape[i].y + piece.position.y][piece.shape[i].x + piece.position.x] = piece.pieceCode;
		}

		piece = Piece(static_cast<Piece::Pieces>(rand() % 7));
	}
}
