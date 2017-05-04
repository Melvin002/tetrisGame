#include <SFML/Graphics.hpp>
#include <string>
#include <queue>
#include "Piece.h"
#include <stdlib.h> 

const int FPS_NUMBER = 60;
const int MULTIPLER = 300;
const int WIDTH = 1 * MULTIPLER;
const int HEIGTH = 2 * MULTIPLER;
const int BOARD_WIDTH = 10;
const int BOARD_HEIGTH = 20;

int board[BOARD_HEIGTH][BOARD_WIDTH] = { 0 };
bool activePiece = false; // tez zmienic bo gowno, mowi czy losowac klocek czy nie
Piece piece; // just initializing no matter with what



enum Directions {
	Left, Right, Top, Bottom, DropDown, SoftDown
};

void processInput(sf::RenderWindow &window);
void update();
void render(sf::RenderWindow &window);
bool move(Directions direction);
void rotate(Directions direction);
void checkIfLanded();
bool isSpaceOccupied(Directions direction, Point shape[], int distance = 1);

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
	

	sf::RenderWindow window;
	window.create(sf::VideoMode(WIDTH, HEIGTH), "Tetris!");
	window.setFramerateLimit(FPS_NUMBER);
	//window.setKeyRepeatEnabled(false);

	sf::Clock clock;
	sf::Time programTime;

	piece = Piece();//piece is global for now

	while (window.isOpen()) {
		
		processInput(window);
		update();
		render(window);

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
			piece = Piece();
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
void update() {
	//piece fall speed
	counter++;
	if (counter > 60) {//60 speed fall limiter
		move(SoftDown);
		counter = 0;
	}

	
	checkIfLanded();


}
void render(sf::RenderWindow &window) {
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
bool move(Directions direction){

	switch (direction) {
	case Left:
		if (!isSpaceOccupied(Left, piece.shape)) {
			piece.position.x--;
			return true;
		}


		break;
	case Right:
		if (!isSpaceOccupied(Right, piece.shape)) {
			piece.position.x++;
			return true;
		}
		break;
	case SoftDown:
		if (!isSpaceOccupied(Bottom, piece.shape)) {
			piece.position.y++;
			return true;
		}
		break;
	case Top:
		if (!isSpaceOccupied(Top, piece.shape)) {
			piece.position.y--;
			return true;
		}
		break;
	}
	
}
//function checks for other pieces blocking the way or end of the board BLOCKED = TRUE, EMPTY = FALSE
bool isSpaceOccupied(Directions direction, Point shape[], int distance) {
	switch (direction) {
	case Left:
		for (int i = 0; i < 4; i++) {
			if (!(shape[i].y + piece.position.y >= 0 && shape[i].y + piece.position.y < BOARD_HEIGTH && shape[i].x + piece.position.x - distance >= 0 && shape[i].x + piece.position.x - distance < BOARD_WIDTH && board[shape[i].y + piece.position.y][shape[i].x + piece.position.x - distance] == 0))
				return true;
		}
		return false;
		break;
	case Right:
		for (int i = 0; i < 4; i++) {
			if (!(shape[i].y + piece.position.y >= 0 && shape[i].y + piece.position.y < BOARD_HEIGTH && shape[i].x + piece.position.x + distance >= 0 && shape[i].x + piece.position.x + distance < BOARD_WIDTH && board[shape[i].y + piece.position.y][shape[i].x + piece.position.x + distance] == 0))
				return true;
		}
		return false;
		break;
	case Bottom:
		for (int i = 0; i < 4; i++) {
			if (!(shape[i].x + piece.position.x >= 0 && shape[i].x + piece.position.x < BOARD_WIDTH && shape[i].y + piece.position.y + distance >= 0 && shape[i].y + piece.position.y + distance < BOARD_HEIGTH && board[shape[i].y + piece.position.y + distance][shape[i].x + piece.position.x] == 0))
				return true;
		}
		return false;
		break;
	case Top:
		for (int i = 0; i < 4; i++) {
			if (!(shape[i].x + piece.position.x >= 0 && shape[i].x + piece.position.x < BOARD_WIDTH && shape[i].y + piece.position.y - distance >= 0 && shape[i].y + piece.position.y - distance < BOARD_HEIGTH && board[shape[i].y + piece.position.y - distance][shape[i].x + piece.position.x] == 0))
				return true;
		}
		return false;
		break;
	}
	return false;
}
void rotate(Directions direction) {

	//ghost for cheking if rotation is possible
	Point tempShape[4];
	bool isRotationPossible = true;
	bool isThereAnObstacle = false;
	switch (direction) {
	case Right:
		if (piece.pieceType != piece.pieceI && piece.pieceType != piece.pieceO) {
			for (int i = 0; i < 4; i++) {
				tempShape[i].y = piece.shape[i].x;
				tempShape[i].x = 2 - piece.shape[i].y;
			}
		}
		else if(piece.pieceType != piece.pieceO){
			for (int i = 0; i < 4; i++) {
				tempShape[i].y = piece.shape[i].x;
				tempShape[i].x = 3 - piece.shape[i].y;
			}
		}
		for (int i = 0; i < 4; i++) {
			if (tempShape[i].x + piece.position.x == -1 || tempShape[i].x + piece.position.x == BOARD_WIDTH) {
				isRotationPossible = false;
			}
		}
		if (!isRotationPossible) {
			if (!isSpaceOccupied(Left, tempShape)) {
				piece.position.x--;
				isRotationPossible = true;
			}
			else if(!isSpaceOccupied(Right, tempShape)) {
				piece.position.x++;
				isRotationPossible = true;
			}
			else if(!isSpaceOccupied(Top, tempShape)){
				piece.position.y--;
				isRotationPossible = true;
			}
			else if (!isSpaceOccupied(Bottom, tempShape)) {
				piece.position.y++;
				isRotationPossible = true;
			}
			else if (!isSpaceOccupied(Left, tempShape, 2)) {
				piece.position.x -= 2;
				isRotationPossible = true;
			}
			else if (!isSpaceOccupied(Right, tempShape, 2)) {
				piece.position.x += 2;
				isRotationPossible = true;
			}
			else if (!isSpaceOccupied(Top, tempShape, 2)) {
				piece.position.y -= 2;
				isRotationPossible = true;
			}
			else if (!isSpaceOccupied(Bottom, tempShape, 2)) {
				piece.position.y += 2;
				isRotationPossible = true;
			}
		}
		if (isRotationPossible && piece.pieceType != piece.pieceO) {
			for (int i = 0; i < 4; i++) {
				piece.shape[i].x = tempShape[i].x;
				piece.shape[i].y = tempShape[i].y;
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
void checkIfLanded() {
	bool pieceLanded = false;
	for (int i = 0; i < 4; i++) {
		if (piece.shape[i].y + piece.position.y + 1 == 20 || board[piece.shape[i].y + piece.position.y + 1][piece.shape[i].x + piece.position.x] != 0)
			pieceLanded = true;
	}
	if (pieceLanded == true) {
		//write piece to board
		for (int i = 0; i < 4; i++) {
			board[piece.shape[i].y + piece.position.y][piece.shape[i].x + piece.position.x] = piece.pieceCode;
		}

		piece = Piece();
	}
}
