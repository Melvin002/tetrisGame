#include <SFML/Graphics.hpp>
#include <string>
#include <queue>
#include "Piece.h"
#include <ctime>  
#include <stdlib.h> 

const int FPS_NUMBER = 60;
const int MULTIPLER = 300;
const int WIDTH = 1 * MULTIPLER;
const int HEIGTH = 2 * MULTIPLER;
const int BOARD_WIDTH = 10;
const int BOARD_HEIGTH = 20;
const int LOCK_DELAY = 120;
const int SHAPE_ARRAY_SIZE = 4;
const int POSITION_OF_PIECE_CODE = 100; // yxx / 100 = y
const int DAS_DELAY = 20; // DAS - delayed auto shift
const int MOVE_AUTOREPEAT_DELAY = 3; //time between each repeat

int board[BOARD_HEIGTH][BOARD_WIDTH] = { 0 };
Piece piece = Piece(Piece::empty);
Piece pieceInHold = Piece(Piece::empty);
sf::RectangleShape rectangle(sf::Vector2f(30, 30));
int ghostOffset;
int lockDelayCounter;
int softDownDAS;
int leftDAS;
int rightDAS;
bool holdHappened = false;


enum Directions {
	Left, Right, Top, Bottom, DropDown, SoftDown
};

void processInput(sf::RenderWindow &window);
void update(sf::RenderWindow &window);
void render(sf::RenderWindow &window);
void move(Directions direction);
void rotate(Directions direction);
void checkIfLanded();
bool isSpaceOccupied(Directions direction, Point shape[], int distance = 1);
void clearLines();
void setPen(sf::RenderWindow &window, Piece::Pieces pieceType, int x, int y);
bool checkIfGameLost();

int main() {
	/*
	piece codes:
	o - 1,	i - 2,	s - 3,	z - 4
	l - 5,	j - 6,	t - 7

	XYYY
	X - type of piece code
	YYY - number of element

	0 - empty
	*/
	std::srand(unsigned(std::time(0)));

	sf::RenderWindow window;
	window.create(sf::VideoMode(WIDTH, HEIGTH), "Tetris!");
	window.setFramerateLimit(FPS_NUMBER);
	window.setKeyRepeatEnabled(false);

	sf::Clock clock;
	sf::Time programTime;

	piece = Piece(Piece::random);//piece is global for now

	while (window.isOpen()) {

		processInput(window);
		update(window);
		render(window);

	}

}
void processInput(sf::RenderWindow &window) {
	//sfml window event capture
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window.close();
		//keyboard input
		if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::X)) {
			rotate(Right);
		}
		if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::LControl || event.key.code == sf::Keyboard::Z)) {
			rotate(Left);
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
			move(DropDown);
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
			//pause in future
		}
		if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::C) && !holdHappened) {
			if (pieceInHold.pieceType == Piece::empty){
				pieceInHold = piece;
				piece = Piece(Piece::random);
			}
			else {
				Piece swapTemp = piece;
				piece = pieceInHold;
				piece.positionReset();
				pieceInHold = swapTemp;
			}
			holdHappened = true;
		}
		if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Left) {
			leftDAS = 0;
		}
		if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Right) {
			rightDAS = 0;
		}
		if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Down) {
			softDownDAS = 0;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		if (leftDAS%DAS_DELAY == 0 && leftDAS <= DAS_DELAY) {
			move(Left);
		}
		else if (leftDAS%MOVE_AUTOREPEAT_DELAY == 0 && leftDAS > DAS_DELAY) {
			move(Left);
			leftDAS = DAS_DELAY;
		}
		leftDAS++;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		if (rightDAS%DAS_DELAY == 0 && rightDAS <= DAS_DELAY) {
			move(Right);
		}
		else if (rightDAS%MOVE_AUTOREPEAT_DELAY == 0 && rightDAS > DAS_DELAY) {
			move(Right);
			rightDAS = DAS_DELAY;
		}
		rightDAS++;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		if (softDownDAS%DAS_DELAY == 0 && softDownDAS <= DAS_DELAY) {
			move(SoftDown);
		}
		else if (softDownDAS%MOVE_AUTOREPEAT_DELAY == 0 && softDownDAS > DAS_DELAY) {
			move(SoftDown);
			softDownDAS = DAS_DELAY;
		}
		softDownDAS++;
	}

}
int counter = 0;
void update(sf::RenderWindow &window) {
	//piece fall speed
	counter++;
	if (counter > 60) {//60 speed fall limiter
		move(SoftDown);
		counter = 0;
	}
	//find ghost bottom offset
	ghostOffset = 0;
	while (!isSpaceOccupied(Bottom, piece.shape, ghostOffset + 1)) {
		ghostOffset++;
	}
	if (isSpaceOccupied(Bottom, piece.shape))
		lockDelayCounter++;
	else
		lockDelayCounter = 0;
	checkIfLanded();
	if(checkIfGameLost())
		window.close();


}
void render(sf::RenderWindow &window) {
	window.clear(sf::Color::Black);
	//przelatuje po planszy i rysuje kwadracik tam gdzie cos ma byc - chujowe trzeba bedzie zmienic
	for (int i = 0; i < BOARD_HEIGTH; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			if(board[i][j] != 0)
				setPen(window, static_cast<Piece::Pieces>((board[i][j] / POSITION_OF_PIECE_CODE) - 1), j * 30, i * 30);
		}
	}
	//render ghost
	for (int i = 0; i < 4; i++) {
		setPen(window, Piece::shadow, (piece.shape[i].x + piece.position.x) * 30, (piece.shape[i].y + piece.position.y + ghostOffset) * 30);
	}
	//render moving piece
	for (int i = 0; i < 4; i++){
		setPen(window, piece.pieceType, (piece.shape[i].x + piece.position.x) * 30, (piece.shape[i].y + piece.position.y) * 30);
	}

	window.display();
}
void setPen(sf::RenderWindow &window, Piece::Pieces pieceType, int x, int y) {
	switch (pieceType) {
	case Piece::pieceO:
		rectangle.setFillColor(sf::Color::Yellow);
		rectangle.setPosition(x, y);
		window.draw(rectangle);
		break;
	case Piece::pieceI:
		rectangle.setFillColor(sf::Color::Cyan);
		rectangle.setPosition(x, y);
		window.draw(rectangle);
		break;
	case Piece::pieceS:
		rectangle.setFillColor(sf::Color::Green);
		rectangle.setPosition(x, y);
		window.draw(rectangle);
		break;
	case Piece::pieceZ:
		rectangle.setFillColor(sf::Color::Red);
		rectangle.setPosition(x, y);
		window.draw(rectangle);
		break;
	case Piece::pieceL:
		rectangle.setFillColor(sf::Color(255, 165, 0));
		rectangle.setPosition(x, y);
		window.draw(rectangle);
		break;
	case Piece::pieceJ:
		rectangle.setFillColor(sf::Color::Blue);
		rectangle.setPosition(x, y);
		window.draw(rectangle);
		break;
	case Piece::pieceT:
		rectangle.setFillColor(sf::Color::Magenta);
		rectangle.setPosition(x, y);
		window.draw(rectangle);
		break;
	case Piece::shadow:
		rectangle.setFillColor(sf::Color::White);
		rectangle.setPosition(x, y);
		window.draw(rectangle);
		break;
	}
}
void move(Directions direction){

	switch (direction) {
	case Left:
		if (!isSpaceOccupied(Left, piece.shape)) {
			piece.position.x--;
		}
		break;
	case Right:
		if (!isSpaceOccupied(Right, piece.shape)) {
			piece.position.x++;
		}
		break;
	case SoftDown:
		if (!isSpaceOccupied(Bottom, piece.shape)) {
			piece.position.y++;
		}
		break;
	case Top:
		if (!isSpaceOccupied(Top, piece.shape)) {
			piece.position.y--;
		}
		break;
	case DropDown:
		while (!isSpaceOccupied(Bottom, piece.shape)) {
			piece.position.y++;
		}
		lockDelayCounter = LOCK_DELAY;
		break;
	}
	
}
//function checks for other pieces blocking the way or end of the board BLOCKED = TRUE, EMPTY = FALSE
bool isSpaceOccupied(Directions direction, Point shape[], int distance) {
	switch (direction) {
	case Left:
		for (int i = 0; i < 4; i++) {
			if (!(shape[i].y + piece.position.y < BOARD_HEIGTH && shape[i].x + piece.position.x - distance >= 0 && shape[i].x + piece.position.x - distance < BOARD_WIDTH && board[shape[i].y + piece.position.y][shape[i].x + piece.position.x - distance] == 0))
				return true;
		}
		return false;
		break;
	case Right:
		for (int i = 0; i < 4; i++) {
			if (!(shape[i].y + piece.position.y < BOARD_HEIGTH && shape[i].x + piece.position.x + distance >= 0 && shape[i].x + piece.position.x + distance < BOARD_WIDTH && board[shape[i].y + piece.position.y][shape[i].x + piece.position.x + distance] == 0))
				return true;
		}
		return false;
		break;
	case Bottom:
		for (int i = 0; i < 4; i++) {
			if (!(shape[i].x + piece.position.x >= 0 && shape[i].x + piece.position.x < BOARD_WIDTH && shape[i].y + piece.position.y + distance < BOARD_HEIGTH && board[shape[i].y + piece.position.y + distance][shape[i].x + piece.position.x] == 0))
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
			if (tempShape[i].x + piece.position.x < 0 || tempShape[i].x + piece.position.x >= BOARD_WIDTH || board[tempShape[i].y + piece.position.y][tempShape[i].x + piece.position.x] != 0) {
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
				tempShape[i].x = piece.shape[i].y;
				tempShape[i].y = 2 - piece.shape[i].x;
			}
		}
		else if (piece.pieceType != piece.pieceO) {
			for (int i = 0; i < 4; i++) {
				tempShape[i].x = piece.shape[i].y;
				tempShape[i].y = 3 - piece.shape[i].x;
			}
		}
		for (int i = 0; i < 4; i++) {
			if (tempShape[i].x + piece.position.x < 0 || tempShape[i].x + piece.position.x >= BOARD_WIDTH || board[tempShape[i].y + piece.position.y][tempShape[i].x + piece.position.x] != 0) {
				isRotationPossible = false;
			}
		}
		if (!isRotationPossible) {
			if (!isSpaceOccupied(Left, tempShape)) {
				piece.position.x--;
				isRotationPossible = true;
			}
			else if (!isSpaceOccupied(Right, tempShape)) {
				piece.position.x++;
				isRotationPossible = true;
			}
			else if (!isSpaceOccupied(Top, tempShape)) {
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
	}
}
void checkIfLanded() {
	if (lockDelayCounter > LOCK_DELAY) {
		//write piece to board
		for (int i = 0; i < 4; i++) {
			board[piece.shape[i].y + piece.position.y][piece.shape[i].x + piece.position.x] = piece.pieceCode;
		}

		clearLines();
		piece = Piece(Piece::random);
		holdHappened = false;
		lockDelayCounter = 0;
	}
}
void clearLines() {
	std::vector<int> indexesOfClearedLines; //store indexes
	int pieceInRowCounter = 0;
	for (int i = 0; i < BOARD_HEIGTH; i++) {
		pieceInRowCounter = 0;
		for (int j = 0; j < BOARD_WIDTH; j++) {
			if (board[i][j] != 0)
				pieceInRowCounter++;
		}
		//find from which indexes lines should be removed
		if (pieceInRowCounter == 10) {
			indexesOfClearedLines.push_back(i);
			pieceInRowCounter = 0;
		}
	}
	if (!indexesOfClearedLines.empty()) {
		int temp = indexesOfClearedLines.size();
		for (int iOld = BOARD_HEIGTH - 1, iNew = BOARD_HEIGTH - 1; iOld >= 0; iOld--, iNew--) {
			//skip indexes that should be romved
			while (!indexesOfClearedLines.empty() && iOld == indexesOfClearedLines.back()) {
				iOld--;
				indexesOfClearedLines.pop_back();
			}
			for (int j = 0; j < BOARD_WIDTH; j++) {
				board[iNew][j] = board[iOld][j];
			}
		}
		for (int i = temp - 1; i >= 0; i--) {
			//clear the rest of board
			for (int j = 0; j < BOARD_WIDTH; j++) {
				board[i][j] = 0;
			}
		}
	}
}
bool checkIfGameLost() {
	int rowCounter = 0;
	for (int j = 0; j < BOARD_WIDTH; j++) {
		if (board[0][j] != 0)
			rowCounter++;
	}
	if (rowCounter > 0)
		return true;
	else 
		return false;
}
